# cat /dev/nyan

Kernel space nyancat chardev for the beloved Linux kernel.

## Usage

```shell
apt install linux-headers-$(uname -r)
make
insmod nyancat.ko
cat /dev/nyan
```
