/* This file is part of footpedald.

    footpedald is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    footpedald is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with footpedald.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _25398a7e_a57e_4090_ab72_ee38da949f68_
#define _25398a7e_a57e_4090_ab72_ee38da949f68_
#pragma once

#ifdef _GNU_SOURCE
#define fpd_get_env secure_getenv
#else
#define fpd_get_env getenv
#endif /* _GNU_SOURCE */

#endif /* _25398a7e_a57e_4090_ab72_ee38da949f68_ */
