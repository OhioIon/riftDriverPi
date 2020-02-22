// ********************* Includes       ********************* //
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "hidapi.h"
#include "rift.h"

// ********************* Defines        ********************* //
#define MAX_STR 255

// ********************* Types          ********************* //

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

  wprintf(L"/// riftDriverPi ///\n");

  // Initialize hidapi library
  hid_init();

  // Open device (Oculus Rift CV1)
  handle = hid_open(0x2833, 0x0031, NULL);
  if( handle == NULL )
  {
    wprintf(L"hid_open() failed\n");
    goto abort;
  }

  // Read the Manufacturer String
  res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
  if( res < 0 ) goto cleanup;
  wprintf(L"Manufacturer  : %S\n", wstr);

  // Read the Product String
  res = hid_get_product_string(handle, wstr, MAX_STR);
  if( res < 0 ) goto cleanup;
  wprintf(L"Product       : %S\n", wstr);

  // Read the Serial Number String
  res = hid_get_serial_number_string(handle, wstr, MAX_STR);
  if( res < 0 ) goto cleanup;
  wprintf(L"Serial Number : %S\n", wstr);

  // Set non-blocking
  res = hid_set_nonblocking(handle, 1);
  if( res < 0 ) goto cleanup;
  wprintf(L"Configured non-blocking transfer\n");

  // Turn the CV1 screens on
  res = rift_send_enable_components(handle,1);
  if( res != 0 ) goto cleanup;
  wprintf(L"Screen enabled\n");

  // Send keep alive
  while(!kbhit())
  {
    res = rift_send_keep_alive(handle);
    if( res < 0 ) goto cleanup;
    msleep( 100 );
  }

  // Turn the CV1 screen off
  rift_send_enable_components(handle, 0);

cleanup:
  // Close down connection
  hid_close(handle);

  // Finalize the hidapi library
abort:
  hid_exit();

  wprintf(L"/// Exit ///\n");

  return 0;
}

// EOF
