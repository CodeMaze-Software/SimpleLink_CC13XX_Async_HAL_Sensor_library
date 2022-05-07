/*
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * You understand and agree that the authors are under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or corrections of defects with regard to the Software.
 * The authors nevertheless reserve the right to update, modify, or discontinue the Software at any time.
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * You agree to cite the Dariusz Adamczyk - Codemaze.eu
 *
 * All rights reserved. Not for commercial use.
 *
 */


#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/uart/UARTCC26X2.h>
#include <ti/drivers/I2C.h>
#include "Board.h"

#include "sensor_api.h"

#define AUTO_MODE

UART_Handle uart;
UART_Params uartParams;
char uartBuf[128];

void sensorApi_Callback(char *optLogger, int value)
{
    (void) value;

    if(optLogger != NULL)
        UART_write(uart, optLogger, strlen(optLogger));
}

void *mainThread(void *arg0)
{
    UART_init();
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.readDataMode = UART_DATA_TEXT;

    uart = UART_open(Board_UART0, &uartParams);

    sprintf((char*) uartBuf, "Async sensor API example\n");
    UART_write(uart, uartBuf, strlen((char*) uartBuf));

    sensorConfig_t mySensor;
    mySensor.min = -10;
    mySensor.max = 25;
    mySensor.priAddress = 0x31; // 0x48 first AS6212 address
    mySensor.secAddress = 0x33; // 0x49 second AS6212 address
    mySensor.threshold_cnt = 10; // (10 cycles * 100 mS timer tick) = 1 Sec threshold

    sensorApi_Init(&mySensor);

#ifdef AUTO_MODE

    sensorApi_SetThresholds(&mySensor);
    sensorApi_AutoPollingEnable(true);

#elif

    sensorApiStatus_t st;
    st = sensorApi_singleRead(mySensor.priAddress);
    st = sensorApi_singleSleep(mySensor.priAddress, true);

#endif

    sensorApi_RegisterLogCallback(sensorApi_Callback);

    while (1)
    {
        UART_write(uart,"Other tasks...\n", strlen("Other tasks...\n"));
        sleep(1);
    }
}
