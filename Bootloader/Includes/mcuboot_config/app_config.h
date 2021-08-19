#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/*
 * Logging
 */

/*
 * If logging is enabled the following functions must be defined by the
 * platform:
 *
 *    MCUBOOT_LOG_MODULE_REGISTER(domain)
 *      Register a new log module and add the current C file to it.
 *
 *    MCUBOOT_LOG_MODULE_DECLARE(domain)
 *      Add the current C file to an existing log module.
 *
 *    MCUBOOT_LOG_ERR(...)
 *    MCUBOOT_LOG_WRN(...)
 *    MCUBOOT_LOG_INF(...)
 *    MCUBOOT_LOG_DBG(...)
 *
 * The function priority is:
 *
 *    MCUBOOT_LOG_ERR > MCUBOOT_LOG_WRN > MCUBOOT_LOG_INF > MCUBOOT_LOG_DBG
 */
#define MCUBOOT_HAVE_LOGGING

/*
 * Only relevant if MCUBOOT_HAVE_LOGGING is defined, otherwise will be ignored
 */

#define MCUBOOT_LOG_ERR_ENABLE
#define MCUBOOT_LOG_WRN_ENABLE
#define MCUBOOT_LOG_INF_ENABLE
#define MCUBOOT_LOG_INTERNAL_ENABLE
#undef MCUBOOT_LOG_DBG_ENABLE

#endif /* __APP_CONFIG_H__ */