/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/*
 * ARMEDIA_VideoAtoms.h
 *
 * Created by n.brulez on 19/08/11
 * Copyright 2011 Parrot SA. All rights reserved.
 *
 */
#ifndef _ARMEDIA_VIDEOATOMS_H_
#define _ARMEDIA_VIDEOATOMS_H_

#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <libARMedia/ARMEDIA_VideoEncapsuler.h>

#define ARMEDIA_VIDEOATOMS_PVAT "pvat"

#define ARMEDIA_ATOM_TAG_SIZE 4

typedef struct
{
  uint64_t size;
  char tag[ARMEDIA_ATOM_TAG_SIZE];
  uint8_t *data;
  uint8_t wide;
} movie_atom_t;

typedef struct
{
    uint64_t size;
    int flag;
    long long offset;
} atom_check_return;

typedef enum
{
    ARMEDIA_VIDEOATOM_MEDIATYPE_VIDEO = 0,
    ARMEDIA_VIDEOATOM_MEDIATYPE_SOUND,
} eARMEDIA_VIDEOATOM_MEDIATYPE;

#define atom_ntohll(x)  swap_uint64(x)

/* Atoms :
LEGEND :
-> specific = specific function to generate this atom
-> empty = use atomFromData (0, "name", NULL);
-> from data = use atomFromData (dataSize, "name", dataPointer);

ftyp -> specific
pvat -> specific
mdat -> specific (include freeAtom if needed)
moov -> empty
 |- mvhd -> specific
 |- trak -> empty
 |   |- tkhd -> specific
 |   \- mdia -> empty
 |       |- mdhd -> specific
 |       |- hdlr -> specific (for mdia)
 |       \- minf -> empty
 |           |- vmhd -> specific
 |           |- hdlr -> specific (for minf)
 |           |- dinf -> empty
 |           |   \- dref -> specific
 |           \- stbl -> empty
 |               |- stsd -> specific
 |               |- stts -> from data
 |               |- stss -> from data (i frame positions as uint32_t network endian)
 |               |- stsc -> specific
 |               |- stsz -> from data (frames sizes as uint32_t network endian)
 |               \- stco -> from data (frames offset as uint32_t network endian)
*/

/* REMINDER : NEVER INCLUDE A MDAT ATOM INTO ANY OTHER ATOM */

/* GENERIC */
movie_atom_t *atomFromData (uint32_t data_size, const char *tag, const uint8_t *data);
void insertAtomIntoAtom (movie_atom_t *container, movie_atom_t **leaf); // will free leaf
int writeAtomToFile (movie_atom_t **atom, FILE *file); // Will free _atom
void freeAtom (movie_atom_t **atom);

/* SPECIFIC */
movie_atom_t *ftypAtomForFormatAndCodecWithOffset (eARMEDIA_ENCAPSULER_VIDEO_CODEC codec, uint32_t *offset);
movie_atom_t *mdatAtomForFormatWithVideoSize (uint64_t videoSize);
movie_atom_t *mvhdAtomFromFpsNumFramesAndDate (uint32_t timescale, uint32_t duration, time_t date);
movie_atom_t *tkhdAtomWithResolutionNumFramesFpsAndDate (uint32_t w, uint32_t h, uint32_t timescale, uint32_t duration, time_t date, eARMEDIA_VIDEOATOM_MEDIATYPE type);
movie_atom_t *mdhdAtomFromFpsNumFramesAndDate (uint32_t timescale, uint32_t duration, time_t date);
movie_atom_t *hdlrAtomForMdia (eARMEDIA_VIDEOATOM_MEDIATYPE type);
movie_atom_t *vmhdAtomGen ();
movie_atom_t *smhdAtomGen ();
movie_atom_t *hdlrAtomForMinf ();
movie_atom_t *drefAtomGen ();
movie_atom_t *stsdAtomWithResolutionAndCodec (uint32_t w, uint32_t h, eARMEDIA_ENCAPSULER_VIDEO_CODEC codec);
movie_atom_t *stsdAtomWithResolutionCodecSpsAndPps (uint32_t w, uint32_t h, eARMEDIA_ENCAPSULER_VIDEO_CODEC codec, uint8_t *sps, uint32_t spsSize, uint8_t *pps, uint32_t ppsSize);
movie_atom_t *stsdAtomWithAudioCodec(eARMEDIA_ENCAPSULER_AUDIO_CODEC codec, eARMEDIA_ENCAPSULER_AUDIO_FORMAT format, uint16_t nchannel, uint16_t freq);
movie_atom_t *stscAtomGen(uint32_t uniqueCount, uint32_t* stscTable, uint32_t nEntries);
movie_atom_t *stszAtomGen(uint32_t uniqueSize, uint32_t* sizeTable, uint32_t nSamples);
movie_atom_t *metadataAtomFromTagAndValue (const char *tag, const char *value);
movie_atom_t *pvatAtomGen(const char *jsonString);

/**
 * @brief Read atom data from a video file into a self alloced array
 * Thid function get the atom data from a video file and convert it to the latest version
 * This function alloc (using vp_os_calloc) the return pointer. Application MUST handle the free of the pointer.
 * @param videoFile Pointer to the video file.
 * @param atom The name of the atom to extract. If an inner atom is required, use / to separate levels.
 * @param dataSize Pointer which will hold the size of the returned data.
 * @return A new, malloc'd, pointer of pvat data filled with data buffer. In case of failure, returns NULL.
 * @note The video FILE* pointer position will be modified by this call.
 */
uint8_t *createDataFromFile (FILE *videoFile, const char *atom, uint32_t *dataSize);

/**
 * @brief Read atom data from atom into a self alloced array
 * This function get the atom data from a video and convert it to the latest version
 * This function alloc (using vp_os_calloc) the return pointer. Application MUST handle the free of the pointer.
 * @param atomBuffer Pointer to the atom data (not including the leading size and atom tag)
 * @param atomSize Size of the atom. This is used to avoid overflow, or if the atom size was set to zero
 * @return A new, malloc'd, pointer of atom data filled with data. In case of failure, returns NULL
 */
uint8_t *createDataFromAtom (uint8_t *atomBuffer, const int atomSize);

/**
 * @brief Read FPS from a given video file
 * This function get the FPS of a given video file
 * @param videoFile Pointer to the video file
 * @return The number of frames per second of the video. Returns zero if we were unable to read the actual value.
 * @note The video FILE* pointer position will be modified by this call
 * @note This call may fail on non-AR.Drone generated videos
 */
uint32_t getVideoFpsFromFile (FILE *videoFile);

/**
 * @brief Read FPS from a given video mdhd atom
 * This function get the FPS of a given video mdhd atom
 * @param mdhdAtom Pointer to the video atom
 * @param atomSize Size of the video atom pointer
 * @return The number of frames per second of the video. Returns zero if we were unable to read the actual value.
 * @note This call may fail on non-AR.Drone generated videos
 */
uint32_t getVideoFpsFromAtom (uint8_t *mdhdAtom, const int atomSize);

/**
 * @brief Read size and atomName from a given buffer of atom
 * This function get the size and the name of a given buffer atom
 * @param[in] buffer Pointer to the buffer atom
 * @param[in|out] offset Pointer of number of bytes offset in the video
 * @param[in] size for the number of bytes of the video
 * @param[in] tag char Pointer of the wanted atom Name
 * @return int return 0 if the tag is not found and increment the off with the atom size else return 1 if success
 */
int seekMediaBufferToAtom (uint8_t *buff, long long *offset,long long size, const char *tag);

/**
 * @brief Read size and atomName from a given video file
 * This function seek a file to a given Atom
 * @param[in] videoFile Pointer to the video file
 * @param[in] tag char Pointer of the wanted atom Name
 * @param[in|out] size Pointer of number of bytes for te wanted atom in the video
 * @param[in] index of the atom (if many atoms have the same name). Index starts at 1
 * @return int return 0 if the tag is not found else return 1 if success and set the size of retAtomSize
 */
int seekMediaFileToAtom (FILE *videoFile, const char *atomName, uint64_t *retAtomSize, uint32_t atomIndex);

/**
 * @brief This function reversed the byte order of a uint64_t
 * @param value in uint64_t
 * @return uint64_t reversed
*/
uint64_t swap_uint64(uint64_t value);

char* ARMEDIA_VideoAtom_GetPVATString(const eARDISCOVERY_PRODUCT id, const char* uuid, const char* runDate, const char* filepath, const struct tm* mediaDate);

#endif // _ARMEDIA_VIDEO_ATOMS_H_
