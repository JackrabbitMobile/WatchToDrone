/**
 * @file arstream2_error.h
 * @brief Parrot Streaming Library - Error definitions
 * @date 04/16/2015
 * @author aurelien.barre@parrot.com
 */

#ifndef _ARSTREAM2_ERROR_H_
#define _ARSTREAM2_ERROR_H_

/**
 * @brief Error codes for ARSTREAM2_xxx calls
 */
typedef enum {
    ARSTREAM2_OK = 0,                           /**< No error */
    ARSTREAM2_ERROR_BAD_PARAMETERS = -1,        /**< Bad parameters */
    ARSTREAM2_ERROR_ALLOC = -2,                 /**< Unable to allocate resource */
    ARSTREAM2_ERROR_BUSY = -3,                  /**< Object is busy and can not be deleted yet */
    ARSTREAM2_ERROR_QUEUE_FULL = -4,            /**< Queue is full */
    ARSTREAM2_ERROR_WAITING_FOR_SYNC = -5,      /**< Waiting for synchronization */
    ARSTREAM2_ERROR_RESYNC_REQUIRED = -6,       /**< Re-synchronization required */
    ARSTREAM2_ERROR_RESOURCE_UNAVAILABLE = -7,  /**< Resource unavailable */
    ARSTREAM2_ERROR_NOT_FOUND = -8,             /**< Not found */
    ARSTREAM2_ERROR_INVALID_STATE = -9,         /**< Invalid state */
    ARSTREAM2_ERROR_UNSUPPORTED = -10,          /**< Unsupported */
} eARSTREAM2_ERROR;

/**
 * @brief Gets the error string associated with an eARSTREAM2_ERROR
 *
 * @param error The error to describe
 * @return A static string describing the error
 *
 * @note User should NEVER try to modify a returned string
 */
char* ARSTREAM2_Error_ToString(eARSTREAM2_ERROR error);

#endif /* _ARSTREAM2_ERROR_H_ */
