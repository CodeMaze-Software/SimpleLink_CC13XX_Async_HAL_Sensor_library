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

/******************************************************************************
 * User sensor library include
 *****************************************************************************/

#include "LIS2DT_lib.h"
//#include "AS6212_lib.h"

/******************************************************************************
 ******************************************************************************
 *****************************************************************************/

#include "sensor_api_user_defines.h"

/******************************************************************************
 *
 *
 * User implementation
 *
 *
 *****************************************************************************/

sensorUserApiStatus_t sensorApi_UserLibrary_Init(uint8_t deviceI2cAddress)
{
    return lis2dtw12Init();
    //return as6212Init();
}

sensorUserApiStatus_t sensorApi_UserLibrary_Read(uint8_t deviceI2cAddress)
{
    return lis2dtw12GetZ(deviceI2cAddress);
    //return as6212GetTemperature(deviceI2cAddress);
}

sensorUserApiStatus_t sensorApi_UserLibrary_Write(uint8_t deviceI2cAddress)
{
    return 0;
}

sensorUserApiStatus_t sensorApi_UserLibrary_Sleep(uint8_t deviceI2cAddress, bool status)
{
    return lis2dtw12SleepMode(deviceI2cAddress, status);
    //return as6212SleepMode(status);
}

/******************************************************************************
 ******************************************************************************
 *****************************************************************************/
