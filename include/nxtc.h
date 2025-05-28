/*
 * nxtc.h
 *
 * Copyright (c) 2025, DarkMatterCore <pabloacurielz@gmail.com>.
 *
 * This file is part of libnxtc (https://github.com/DarkMatterCore/libnxtc).
 */

#pragma once

#ifndef __NXTC_H__
#define __NXTC_H__

#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Library version.
#define LIBNXTC_VERSION_MAJOR   0
#define LIBNXTC_VERSION_MINOR   0
#define LIBNXTC_VERSION_MICRO   1

/// Provides a way for the library to return application metadata for a specific title.
typedef struct {
    u64 title_id;                   ///< Title ID from the application / system title this data belongs to.
    NacpLanguageEntry lang_entry;   ///< UTF-8 strings in the console's language.
    size_t icon_size;               ///< JPEG icon size.
    void *icon_data;                ///< JPEG icon data.
} NxTitleCacheApplicationMetadata;

/// Initializes the title cache interface by loading and parsing the title cache file from the SD card.
/// Returns false if an error occurs.
bool nxtcInitialize(void);

/// Frees the internal title cache, flushes the title cache file and closes the title cache interface.
void nxtcExit(void);

/// Checks if the provided title ID exists within the internal title cache.
/// Returns false if the provided title ID can't be found, if the title cache interface hasn't been initialized or if the internal title cache is empty.
bool nxtcCheckIfEntryExists(u64 title_id);

/// Provides a pointer to a dynamically allocated NxTitleCacheApplicationMetadata element with data from the internal title cache.
/// Returns NULL if the provided title ID doesn't exist within the internal title cache or if an error occurs.
NxTitleCacheApplicationMetadata *nxtcGetApplicationMetadataEntryById(u64 title_id);

/// Updates the internal title cache to add a new entry with information from the provided arguments.
/// Input strings must be NULL-terminated. The specified icon size must never exceed 0x20000 bytes.
/// Returns true if the new entry has been successfully added, or if the title ID already exists within the internal title cache (unless `force_add` is set to true).
/// Returns false if an error occurs.
bool nxtcAddEntry(u64 title_id, const char *name, const char *publisher, size_t icon_size, const void *icon_data, bool force_add);

/// Flushes the title cache file to the SD card if there's any pending changes for it.
void nxtcFlushCacheFile(void);

/// Frees a NxTitleCacheApplicationMetadata element.
NX_INLINE void nxtcFreeApplicationMetadata(NxTitleCacheApplicationMetadata **app_metadata)
{
    NxTitleCacheApplicationMetadata *ptr = NULL;
    if (!app_metadata || !(ptr = *app_metadata)) return;
    if (ptr->icon_data) free(ptr->icon_data);
    free(ptr);
    *app_metadata = NULL;
}

#ifdef __cplusplus
}
#endif

#endif /* __NXTC_H__ */
