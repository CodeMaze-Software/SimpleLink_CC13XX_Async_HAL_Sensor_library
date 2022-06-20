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

#ifndef SENSOR_I2C_H_
#define SENSOR_I2C_H_

#include <stdint.h>
#include <stddef.h>
#include <ti/drivers/I2C.h>

typedef int sensorSpiStatus_t;

enum
{
    I2C_ERR = 0, I2C_OK
};

enum
{
    I2C_NOT_INITIALIZED, I2C_INITIALIZED_OK
};

sensorSpiStatus_t
sensorI2cAsyncInit(void (*i2cTransactionCallback)(I2C_Handle handle, I2C_Transaction *transaction,
        bool transferStatus));

sensorSpiStatus_t
sensorI2cAsyncGetInitStatus();

sensorSpiStatus_t
sensorI2cAsyncWriteData(uint8_t addr, uint8_t *dataTxBuffer, uint8_t txCnt);

sensorSpiStatus_t
sensorI2cAsyncReadData(uint8_t addr, uint8_t cmd, uint8_t *dataRxBuffer,
                  uint8_t rxCnt);

sensorSpiStatus_t
sensorI2cAsyncTransferData(uint8_t addr, uint8_t *dataTxBuffer, uint8_t txCnt,
                      uint8_t *dataRxBuffer, uint8_t rxCnt);

#endif /* SENSOR_I2C_H_ */
