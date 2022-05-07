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

#include <AS6212_lib.h>

#include "sensor_api.h"
#include "sensor_i2c.h"

uint8_t txBuffer[1];
uint8_t rxBuffer[2];
uint16_t result;

#define GET_TEMPERATURE_REG 0x00

void as6212I2cCallback(I2C_Handle handle, I2C_Transaction *transaction,
                 bool transferStatus)
{
    result = (rxBuffer[0] << 8 | rxBuffer[1]);
    result *= 0.0078125;

    if (transferStatus)
        sensorApi_onResult(result);
}

bool as6212Init(void)
{
    return sensorI2cAsyncInit(as6212I2cCallback);
}

bool as6212GetTemperature(uint8_t i2cAddress)
{
    return sensorI2cAsyncReadData(i2cAddress, GET_TEMPERATURE_REG, rxBuffer, 2);
}

bool as6212SleepMode(bool enabled)
{
    //todo
    return 0;
}

