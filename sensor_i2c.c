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

#include "sensor_i2c.h"
#include <Board.h>

sensorSpiStatus_t initialized;

I2C_Handle spi;
I2C_Params spiParams;
I2C_Transaction spiTransaction;

sensorSpiStatus_t sensorI2cAsyncInit(
        void (*i2cTransactionCallback)(I2C_Handle handle,
                                       I2C_Transaction *transaction,
                                       bool transferStatus))
{
    initialized = false;

    if (i2cTransactionCallback == NULL)
        return I2C_ERR;

    I2C_init();
    I2C_Params_init(&spiParams);

    spiParams.bitRate = I2C_400kHz;
    spiParams.transferMode = I2C_MODE_CALLBACK;
    spiParams.transferCallbackFxn = i2cTransactionCallback;

    spi = I2C_open(CC1310_LAUNCHXL_I2C0, &spiParams);
    if (spi == NULL)
    {
        return I2C_ERR;
    }

    initialized = true;
    return I2C_OK;
}

sensorSpiStatus_t sensorI2cAsyncGetInitStatus()
{
    return initialized;
}

sensorSpiStatus_t sensorI2cAsyncWriteData(uint8_t addr, uint8_t *dataTxBuffer,
                                          uint8_t txCnt)
{
    return sensorI2cAsyncTransferData(addr, dataTxBuffer, txCnt, NULL, 0);
}

sensorSpiStatus_t sensorI2cAsyncReadData(uint8_t addr, uint8_t cmd,
                                         uint8_t *dataRxBuffer, uint8_t rxCnt)
{
    return sensorI2cAsyncTransferData(addr, &cmd, 1, dataRxBuffer, rxCnt);
}

sensorSpiStatus_t sensorI2cAsyncTransferData(uint8_t addr,
                                             uint8_t *dataTxBuffer,
                                             uint8_t txCnt,
                                             uint8_t *dataRxBuffer,
                                             uint8_t rxCnt)
{
    spiTransaction.writeBuf = dataTxBuffer;
    spiTransaction.writeCount = txCnt;
    spiTransaction.readBuf = dataRxBuffer;
    spiTransaction.readCount = rxCnt;

    spiTransaction.slaveAddress = addr;
    if (!I2C_transfer(spi, &spiTransaction))
    {
        return I2C_ERR;
    }

    return I2C_OK;
}

