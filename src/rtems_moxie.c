#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include <inttypes.h>
#include <rtems.h>
#include <rtems/imfs.h>
#include <rtems/malloc.h>
#include <rtems/libcsupport.h>

#include "fsdata.h"

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            2
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 100
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#include <rtems/confdefs.h>

rtems_task Init(rtems_task_argument argument);
rtems_task Application_task(rtems_task_argument argument);
int rtems_tty_getch(void);


rtems_task Init(rtems_task_argument argument)
{
  rtems_name        task_name;
  rtems_id          tid;


  printf( "Creating and starting application task\n" );

  task_name = rtems_build_name('T', 'A', '1', ' ');

  rtems_task_create(task_name, 1, RTEMS_MINIMUM_STACK_SIZE,
		    RTEMS_INTERRUPT_LEVEL(0), RTEMS_DEFAULT_ATTRIBUTES, 
		    &tid);

  rtems_task_start(tid, Application_task, 0);

  rtems_task_delete(RTEMS_SELF);
}


void game_exit(void)
{
  printf ("\n\n\n\nGame ended - simulator is halted!\n\n\n");
  fflush(stdout);
  while(1);
}


rtems_task Application_task(rtems_task_argument argument)
{
  extern int main(int argc, char *argv[]);


  atexit(game_exit);

  putenv ("TERM=xterm");
  
  printf("rtems_tarfs_load returns %d\n", 
	 rtems_tarfs_load("/", tarfile_data, sizeof(tarfile_data)));

  printf("\n\n\nWelcome to Moxie NetHack!\n\n");
  printf("Press any key to start the game\n\n\n\n");

  rtems_tty_getch(); // This is used to seed the RNG before starting the game

  main(0, NULL);

  exit(0);
}


#define MARIN_UART_BASE    0xF0000008
#define MARIN_UART_RXRDY   (MARIN_UART_BASE + 0)
#define MARIN_UART_TXRDY   (MARIN_UART_BASE + 2)
#define MARIN_UART_RXDATA  (MARIN_UART_BASE + 4)
#define MARIN_UART_TXDATA  (MARIN_UART_BASE + 6)

int rtems_tty_getch(void)
{
  int i, tty_idle_ctr;
  volatile unsigned short *pUartRxReady = 
    (volatile unsigned short *) MARIN_UART_RXRDY;
  volatile unsigned short *pUartRxData = 
    (volatile unsigned short *) MARIN_UART_RXDATA;


  fflush(stdout);

  while (*pUartRxReady != 1) 
    tty_idle_ctr++;
  i = *pUartRxData;
 
  // As of now we don't have any source for the RNG seed other than the 
  // keypress delay from the user
  srand48(tty_idle_ctr); 

  if (i == 13) i = 10;

  return i;
}


