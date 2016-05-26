#ifndef _25398a7e_a57e_4090_ab72_ee38da949f68_
#define _25398a7e_a57e_4090_ab72_ee38da949f68_
#pragma once

#ifdef _GNU_SOURCE
#define fpd_get_env secure_getenv
#else
#define fpd_get_env getenv
#endif /* _GNU_SOURCE */

#endif /* _25398a7e_a57e_4090_ab72_ee38da949f68_ */
