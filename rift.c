// ********************* Includes       ********************* //
#include "rift.h"

// ********************* Defines        ********************* //

// ********************* Types          ********************* //

// ********************* Data           ********************* //

// ********************* Prototypes     ********************* //

// ********************* Implementation ********************* //

int16_t rift_send_keep_alive( hid_device *handle )
{
  // Prepare buffer
  uint8_t buf_au8[5];
  const uint16_t tiKeepAlive_ms_cu16 = 10 * 1000;
  buf_au8[0] = 0x08; // Keep alive command
  buf_au8[1] = 0x00;
  buf_au8[2] = 0x00;
  buf_au8[3] = tiKeepAlive_ms_cu16 & 0xFF; // Keep alive interval
  buf_au8[4] = tiKeepAlive_ms_cu16 >> 8;   // Keep alive interval

  // Send feature report
  int16_t res_s16 = hid_send_feature_report(handle, buf_au8, sizeof(buf_au8) );
  if( res_s16 < 0 )
  {
    // Error
    return res_s16;
  }

  return 0;
}

int16_t rift_send_enable_components( hid_device *handle, uint8_t on_u8 )
{
  // Prepare buffer
  uint8_t buf_au8[4];
  buf_au8[0] = 0x1d; // Enable components command
  buf_au8[1] = 0x00;
  buf_au8[2] = 0x00;
  buf_au8[3] = (on_u8 != 0)? 0x01 : 0x00; // Flags: 1 = Display, 2 = Audio, 4 = LEDs

  // Send feature report
  int16_t res_s16 = hid_send_feature_report(handle, buf_au8, sizeof(buf_au8) );
  if( res_s16 < 0 )
  {
    // Error
    return res_s16;
  }

  return 0;
}

// EOF
