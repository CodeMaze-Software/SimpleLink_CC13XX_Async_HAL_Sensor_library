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

#ifndef SENSOR_API_H_
#define SENSOR_API_H_

#include <stdbool.h>
#include <stdint.h>

typedef int sensorApiStatus_t;

typedef struct
{
    int min, max;
    uint8_t threshold_cnt;
    uint8_t priAddress, secAddress;

} sensorConfig_t;

sensorApiStatus_t sensorApi_Init(sensorConfig_t *sensor);
sensorApiStatus_t sensorApi_AutoPollingEnable(bool state);
sensorApiStatus_t sensorApi_SetThresholds(sensorConfig_t *sensor);
sensorApiStatus_t sensorApi_RegisterLogCallback(
        void (*appCallback)(char * logInfo, int value));

sensorApiStatus_t sensorApi_singleRead(uint8_t sensorI2cAddress);
sensorApiStatus_t sensorApi_singleWrite(uint8_t sensorI2cAddress);
sensorApiStatus_t sensorApi_singleSleep(uint8_t sensorI2cAddress, bool state);

void sensorApi_onResult(int result);


#endif /* SENSOR_API_H_ */
