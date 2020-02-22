#ifndef _RIFT_H
#define _RIFT_H

// ********************* Includes       ********************* //
#include <stdint.h>
#include "hidapi.h"

// ********************* Defines        ********************* //

// ********************* Types          ********************* //

// ********************* Data           ********************* //

// ********************* Interface      ********************* //

uint8_t rift_send_keep_alive( hid_device *handle );
uint8_t rift_send_enable_components( hid_device *handle, uint8_t on_u8 );

#endif
