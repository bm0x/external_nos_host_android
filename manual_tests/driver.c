/* Copyright 2017 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/types.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "driver.h"

/*****************************************************************************/
/* TODO: #include <linux/citadel.h> */
#define CITADEL_IOC_MAGIC               'c'
struct citadel_ioc_tpm_datagram {
	__u64		buf;
	__u32		len;
	__u32		command;
};
#define CITADEL_IOC_TPM_DATAGRAM	_IOW(CITADEL_IOC_MAGIC, 1, \
					     struct citadel_ioc_tpm_datagram)
/*****************************************************************************/

#define DEV_CITADEL "/dev/citadel0"

device_t *OpenDev(const char *device)
{
	int fd, *new_fd;

	fd = open(device ? device : DEV_CITADEL, O_RDWR);
	if (fd < 0) {
		perror("can't open device");
		return 0;
	}

	new_fd = (int *)malloc(sizeof(int));
	if (!new_fd) {
		perror("can't malloc new fd");
		close(fd);
		return 0;
	}
	*new_fd = fd;
	return new_fd;
}

void CloseDev(device_t *device)
{
	int fd;

	if (!device) {
		fprintf(stderr, "%s: invalid (NULL) device\n", __func__);
		return;
	}
	fd = *(int *)device;
	if (fd < 0) {
		fprintf(stderr, "%s: invalid device\n", __func__);
		return;
	}

	if (close(fd) < 0)
		perror("Problem closing device (ignored)");
	free(device);
}

static uint8_t in_buf[MAX_DEVICE_TRANSFER];
int ReadDev(device_t *device, uint32_t command, uint8_t *buf, uint32_t len)
{
	struct citadel_ioc_tpm_datagram dg = {
		.buf = (unsigned long)in_buf,
		.len = len,
		.command = command,
	};
	int ret;
	int fd;

	if (!device) {
		fprintf(stderr, "%s: invalid (NULL) device\n", __func__);
		return -1;
	}
	fd = *(int *)device;
	if (fd < 0) {
		fprintf(stderr, "%s: invalid device\n", __func__);
		return -2;
	}

	if (len > MAX_DEVICE_TRANSFER) {
		fprintf(stderr, "%s: invalid len (%d > %d)\n", __func__,
			len, MAX_DEVICE_TRANSFER);
		return -3;
	}

	ret = ioctl(fd, CITADEL_IOC_TPM_DATAGRAM, &dg);
	if (ret < 0) {
		perror("can't send spi message");
		return ret;
	}

	memcpy(buf, in_buf, len);

	return 0;
}

static uint8_t out_buf[MAX_DEVICE_TRANSFER];
int WriteDev(device_t *device, uint32_t command, uint8_t *buf, uint32_t len)
{
	struct citadel_ioc_tpm_datagram dg = {
		.buf = (unsigned long)out_buf,
		.len = len,
		.command = command,
	};
	int ret;
	int fd;

	if (!device) {
		fprintf(stderr, "%s: invalid (NULL) device\n", __func__);
		return -1;
	}
	fd = *(int *)device;
	if (fd < 0) {
		fprintf(stderr, "%s: invalid device\n", __func__);
		return -2;
	}

	if (len > MAX_DEVICE_TRANSFER) {
		fprintf(stderr, "%s: invalid len (%d > %d)\n", __func__,
			len, MAX_DEVICE_TRANSFER);
		return -3;
	}

	memcpy(out_buf, buf, len);

	ret = ioctl(fd, CITADEL_IOC_TPM_DATAGRAM, &dg);
	if (ret < 0) {
		perror("can't send spi message");
		return ret;
	}

	return 0;
}
