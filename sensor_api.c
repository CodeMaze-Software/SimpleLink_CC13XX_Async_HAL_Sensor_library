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

#include "sensor_api.h"
#include "sensor_api_user_defines.h"

#include <stdio.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/timer/GPTimerCC26XX.h>
#include <ti/drivers/PIN.h>

GPTimerCC26XX_Handle hTimer;
PIN_Handle outputPinHandle;
PIN_State outputPinState;
PIN_Config outputPinTable[] = {
IOID_6 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                                IOID_7 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH
                                        | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                                PIN_TERMINATE };
void (*callback)(char * logData, int value);
char logBuffer[128];

enum
{
    SENSOR_STRUCT_ERR = (-4),
    SENSOR_CALLBACK_REGISTER_ERR,
    SENSOR_INDICATOR_PIN_ERR,
    SENSOR_SCHEDULER_ERR,
    SENSOR_UNKNOWN_ERR,
    SENSOR_OK
};

typedef enum
{
    priSensorWake,
    priSensorRead,
    priSensorSleep,
    secSensorWake,
    secSensorRead,
    secSensorSleep,

} sensorWorkflowMode_t;

sensorWorkflowMode_t state;

typedef struct
{
    int api_sensorType;
    int api_min;
    int api_max;
    uint8_t api_threshold_cnt;
    uint8_t api_priAddress, api_secAddress;

} apiSensorConfig_t;

apiSensorConfig_t config;

uint8_t min_threshold_cnt_value, max_threshold_cnt_value;

void sensorScheduler(GPTimerCC26XX_Handle handle,
                     GPTimerCC26XX_IntMask interruptMask)
{
    switch (state)
    {
    case priSensorWake:
        sensorApi_UserLibrary_Sleep(config.api_priAddress, false);
        break;

    case priSensorRead:
        sensorApi_UserLibrary_Read(config.api_priAddress);
        break;

    case priSensorSleep:
        sensorApi_UserLibrary_Sleep(config.api_priAddress, true);
        break;

    case secSensorWake:
        sensorApi_UserLibrary_Sleep(config.api_priAddress, false);
        break;

    case secSensorRead:
        sensorApi_UserLibrary_Read(config.api_secAddress);
        break;

    case secSensorSleep:
        sensorApi_UserLibrary_Sleep(config.api_priAddress, true);
        break;
    }

    if (state++ >= secSensorSleep)
        state = priSensorWake;

    PIN_setOutputValue(outputPinHandle, IOID_6, !PIN_getOutputValue(IOID_6));
}

sensorApiStatus_t sensorApi_Init(sensorConfig_t *sensor)
{
    if (sensor == NULL)
        return SENSOR_STRUCT_ERR;

    config.api_min = sensor->min;
    config.api_max = sensor->max;
    config.api_priAddress = sensor->priAddress;
    config.api_secAddress = sensor->secAddress;
    config.api_threshold_cnt = sensor->threshold_cnt;

    xdc_runtime_Types_FreqHz freq;
    GPTimerCC26XX_Params params;
    GPTimerCC26XX_Params_init(&params);
    params.width = GPT_CONFIG_32BIT;
    params.mode = GPT_MODE_PERIODIC_UP;
    params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF;
    hTimer = GPTimerCC26XX_open(GPT_A, &params);
    if (hTimer == NULL)
    {
        return SENSOR_SCHEDULER_ERR;
    }

    BIOS_getCpuFreq(&freq);
    GPTimerCC26XX_Value loadVal = freq.lo / 10;
    GPTimerCC26XX_setLoadValue(hTimer, loadVal);
    GPTimerCC26XX_registerInterrupt(hTimer,
                                    (GPTimerCC26XX_HwiFxn) &sensorScheduler,
                                    GPT_INT_TIMEOUT);

    outputPinHandle = PIN_open(&outputPinState, outputPinTable);
    if (outputPinHandle == NULL)
    {
        return SENSOR_INDICATOR_PIN_ERR;
    }

    sensorApi_UserLibrary_Init(0);

    return SENSOR_OK;
}

sensorApiStatus_t sensorApi_AutoPollingEnable(bool state)
{
    state ? GPTimerCC26XX_start(hTimer) : GPTimerCC26XX_stop(hTimer);
    return SENSOR_OK;
}
sensorApiStatus_t sensorApi_SetThresholds(sensorConfig_t *sensor)
{
    config.api_min = sensor->min;
    config.api_max = sensor->max;

    return SENSOR_OK;
}
sensorApiStatus_t sensorApi_RegisterLogCallback(
        void (*appCallback)(char * logInfo, int value))
{
    if (appCallback != NULL)
    {
        callback = appCallback;
        return SENSOR_OK;
    }
    else
        return SENSOR_CALLBACK_REGISTER_ERR;
}

sensorApiStatus_t sensorApi_singleRead(uint8_t sensorI2cAddress)
{
    return sensorApi_UserLibrary_Read(sensorI2cAddress);
}

sensorApiStatus_t sensorApi_singleWrite(uint8_t sensorI2cAddress)
{
    return sensorApi_UserLibrary_Write(sensorI2cAddress);
}

sensorApiStatus_t sensorApi_singleSleep(uint8_t sensorI2cAddress, bool state)
{
    return sensorApi_UserLibrary_Sleep(sensorI2cAddress, state);
}

void sensorApi_onResult(int result)
{
    if (callback != NULL && result < config.api_min)
    {
        min_threshold_cnt_value++;

        if (min_threshold_cnt_value > config.api_threshold_cnt)
        {
            sprintf(logBuffer, "[FACE-UP] Value: %d\nMin: %d, Max: %d\n",
                    result, config.api_min, config.api_max);

            callback(logBuffer, result);
            min_threshold_cnt_value = 0;
        }
    }
    else if (callback != NULL && result > config.api_max)
    {
        max_threshold_cnt_value++;

        if (max_threshold_cnt_value > config.api_threshold_cnt)
        {
            sprintf(logBuffer, "[FACE-DOWN] Value: %d\nMin: %d, Max: %d\n",
                    result, config.api_min, config.api_max);

            callback(logBuffer, result);
            max_threshold_cnt_value = 0;
        }
    }
    else
    {
        min_threshold_cnt_value = max_threshold_cnt_value = 0;

        callback(NULL, result);
    }

    PIN_setOutputValue(outputPinHandle, IOID_7, !PIN_getOutputValue(IOID_7));
}

