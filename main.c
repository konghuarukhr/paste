#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KEY_MAX_LEN 8
#define VAL_MAX_LEN (4 * 1024)

/**
 * len|key|idx
 * len|val|pad
 */
//void write_kv()

void write_log(int fd,
		const char key[], int key_len,
		const char val[], int val_len)
{
	char buf[2 + KEY_MAX_LEN + VAL_MAX_LEN];
	*(uint16_t *)buf = (key_len << 12) + (val_len);
	memcpy(buf + 2, key, key_len);
	memcpy(buf + 2 + key_len, val, val_len);
	write(fd, buf, 2 + key_len + val_len);
	fsync(fd);
}

void read_log(int fd,
		char key[], int *key_len,
		char val[], int *val_len)
{
	uint16_t len;
	read(fd, &len, 2);
	*key_len = len >> 12;
	*val_len = len & 0x3FF;
	read(fd, key, *key_len);
	read(fd, val, *val_len);
}

int main(int argc, char *argv[])
{
	int fd = open("append.log", O_WRONLY | O_CREAT | O_APPEND);
	write_log(fd, "", 0, "111", 3);
	write_log(fd, "a", 1, "222", 3);
	write_log(fd, "bb", 2, "333", 3);
	write_log(fd, "ccc", 3, "444", 3);
	close(fd);

	char key[KEY_MAX_LEN + 1];
	int key_len;
	char val[VAL_MAX_LEN + 1];
	int val_len;
	fd = open("append.log", O_RDONLY | O_CREAT);
	read_log(fd, key, &key_len, val, &val_len);
	key[key_len] = 0;
	val[val_len] = 0;
	printf("%s %s\n", key, val);
	read_log(fd, key, &key_len, val, &val_len);
	key[key_len] = 0;
	val[val_len] = 0;
	printf("%s %s\n", key, val);
	read_log(fd, key, &key_len, val, &val_len);
	key[key_len] = 0;
	val[val_len] = 0;
	printf("%s %s\n", key, val);
	read_log(fd, key, &key_len, val, &val_len);
	key[key_len] = 0;
	val[val_len] = 0;
	printf("%s %s\n", key, val);
	close(fd);

	return 0;
}
