/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <errno.h>
#include <zephyr/drivers/sensor.h>

#include "battery_common.h"

int battery_channel_get(const struct battery_value *value, enum sensor_channel chan,
                        struct sensor_value *val_out) {
    switch (chan) {
    case SENSOR_CHAN_GAUGE_VOLTAGE:
        val_out->val1 = value->millivolts / 1000;
        val_out->val2 = (value->millivolts % 1000) * 1000U;
        break;

    case SENSOR_CHAN_GAUGE_STATE_OF_CHARGE:
        val_out->val1 = value->state_of_charge;
        val_out->val2 = 0;
        break;

    default:
        return -ENOTSUP;
    }

    return 0;
}

uint8_t lithium_ion_mv_to_pct(int16_t bat_mv) {
    if (bat_mv >= 4150) {
        return 100;
    } else if (bat_mv > 4025) {
        return (bat_mv - 4025) * 5.0 / (4150.0 - 4025.0) + 95;
    } else if (bat_mv > 3650) {
        return (bat_mv - 3650) * 65.0 / (4025.0 - 3650.0) + 30;
    } else if (bat_mv > 3400) {
        return (bat_mv - 3400) * 25.0 / (3650.0 - 3400.0) + 5;
    } else if (bat_mv > 3200) {
        return (bat_mv - 3200) * 5.0 / (3400.0 - 3200.0);
    } else {
        return 0;
    }
}