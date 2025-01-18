// ********************* Includes       ********************* //
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

#include "hidapi.h"
#include "rift.h"

// ********************* Defines        ********************* //
#define MAX_STR 255

// ********************* Types          ********************* //
typedef enum state_e
{
  state_Init_E = 0,  // Wait for Rift to be connected. Init once found.
  state_KeepAlive_E, // Send keep alive messages
  state_Cleanup_E,   // Cleanup and restart
}state_te;

// ********************* Data           ********************* //

// ********************* Prototypes     ********************* //

// ********************* Implementation ********************* //

int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0)
  {
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do
  {
    res = nanosleep(&ts, &ts);
  } while (res);

  return res;
}

int kbhit()
{
  #define STDIN_FILENO 0
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

int main(int argc, char* argv[])
{
  int res;
  wchar_t wstr[MAX_STR];
  hid_device *handle;
  state_te state_e = state_Init_E;

  wprintf(L"/// riftDriverPi V2.0.0 ///\n");
  
  // Check if program is started with terminal attached
  _Bool flgIsTerminal_l = isatty(STDIN_FILENO);
  if( flgIsTerminal_l)
  {
    wprintf(L"Terminal mode enabled. Hit 'return' to exit.\n");
  }
  wprintf(L"Waiting for device ...\n");

  // Initialize hidapi library
  hid_init();
  
  while(1)
  {
    // Execute main loop once every second
    msleep( 1000 );
    
    // In terminal mode, exit on "return" keyboard hit
    if( flgIsTerminal_l )
    {
      if( kbhit() )
      {
        // Disable screen immediately (not waiting for timeout of HMD)
        if( (state_e == state_KeepAlive_E) && (handle != NULL) )
        {
          // Turn the CV1 screen off
          rift_send_enable_components(handle, 0);
          wprintf(L"Screen turned OFF\n");
        }
        
        // Stop main loop
        break;
      }
    }
    
    // Primary state machine
    switch(state_e)
    {
      case state_Init_E: ///////////////////////////////////////////////
      
        // Open device (Oculus Rift CV1)
        handle = hid_open(0x2833, 0x0031, NULL);
        if( handle == NULL )
        {
          // Rift not found, abort init and try next time
          continue;
        }
        else
        {
          wprintf(L"Device found!\n");
        }
        
        // Read the Manufacturer String
        res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
        if( res < 0 )
        {
          state_e = state_Cleanup_E;
          continue;
        }
        wprintf(L"Manufacturer  : %S\n", wstr);
        
        // Read the Product String
        res = hid_get_product_string(handle, wstr, MAX_STR);
        if( res < 0 )
        {
          state_e = state_Cleanup_E;
          continue;
        }
        wprintf(L"Product       : %S\n", wstr);

        // Read the Serial Number String
        res = hid_get_serial_number_string(handle, wstr, MAX_STR);
        if( res < 0 )
        {
          state_e = state_Cleanup_E;
          continue;
        }
        wprintf(L"Serial Number : %S\n", wstr);

        // Set non-blocking
        res = hid_set_nonblocking(handle, 1);
        if( res < 0 )
        {
          state_e = state_Cleanup_E;
          continue;
        }
        wprintf(L"Configured non-blocking transfer\n");
        
        // Turn the CV1 screens on
        res = rift_send_enable_components(handle,1);
        if( res < 0 )
        {
          state_e = state_Cleanup_E;
          continue;
        }
        wprintf(L"Screen turned ON\n");
        
        // Init success, go to keep alive state
        state_e = state_KeepAlive_E;
        
        break;
        
      case state_KeepAlive_E: //////////////////////////////////////////
      
        // Send keep alive message to keep the screen running
        res = rift_send_keep_alive(handle);
        if( res < 0 )
        {
          state_e = state_Cleanup_E;
          continue;
        }
      
        break;
        
      case state_Cleanup_E: ////////////////////////////////////////////
        
        // Close down connection
        hid_close(handle);
        handle = NULL;
        
        wprintf(L"Connection lost. Waiting for device ...\n");
        
        // Go back to init
        state_e = state_Init_E;
        
      default:
        // Should not happen
        state_e = state_Init_E;
        break;
    }
  }
  
  // Close down connection (if any)
  if( handle != NULL )
  {
    hid_close(handle);
  }

  // Finalize the hidapi library
  hid_exit();

  wprintf(L"/// Exit ///\n");

  return 0;
}

// EOF
