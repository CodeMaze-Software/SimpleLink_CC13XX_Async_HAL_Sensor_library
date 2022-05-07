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

#include "LIS2DT_lib.h"

#include "sensor_api.h"
#include "sensor_i2c.h"

#define Z_AXIS_REG  0x2d
#define SLEEP_REG   0x34

#define m_SLEEP_ON  0b01000000
#define m_SLEEP_OFF 0b00000000

uint8_t txBuffer[1];
uint8_t rxBuffer[2];
uint16_t result;

void lis2dtwI2cCallback(I2C_Handle handle, I2C_Transaction *transaction,
                        bool transferStatus)
{
    result = rxBuffer[0];

    if (transferStatus)
        sensorApi_onResult(result);
}

bool lis2dtw12Init(void)
{
    return sensorI2cAsyncInit(lis2dtwI2cCallback);
}
bool lis2dtw12GetZ(uint8_t i2cAddress)
{
    return sensorI2cAsyncReadData(i2cAddress, Z_AXIS_REG, rxBuffer, 1);
}
bool lis2dtw12SleepMode(uint8_t i2cAddress, bool enabled)
{
    if (enabled)
        txBuffer[0] = m_SLEEP_ON;
    else
        txBuffer[1] = m_SLEEP_OFF;

    return sensorI2cAsyncWriteData(i2cAddress, txBuffer, 1);
}

