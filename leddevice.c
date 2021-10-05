#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/types.h>
#include <linux/ide.h>
#include <linux/irq.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>

#define CCM_CCGR1_BASE          (0X020C406C)                                                                                                         
#define SW_MUX_GPIO1_IO03_BASE  (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE  (0X020E02F4)
#define GPIO1_DR_BASE           (0X0209C000)
#define GPIO1_DIR_BASE          (0X0209C004)

#define REGISTER_LENGTH         4

static struct resource led_resources[5] = {
        [0] = {
                .start = CCM_CCGR1_BASE,
                .end = CCM_CCGR1_BASE + REGISTER_LENGTH - 1,
                .flags = IORESOURCE_MEM,
        },
        [1] = {
                .start = SW_MUX_GPIO1_IO03_BASE,
                .end = SW_PAD_GPIO1_IO03_BASE + REGISTER_LENGTH - 1,
                .flags = IORESOURCE_MEM,
        },
        [2] = {
                .start = SW_PAD_GPIO1_IO03_BASE,
                .end = SW_PAD_GPIO1_IO03_BASE + REGISTER_LENGTH - 1,
                .flags = IORESOURCE_MEM,
        },
        [3] = {
                .start = GPIO1_DR_BASE,
                .end = GPIO1_DR_BASE + REGISTER_LENGTH - 1,
                .flags = IORESOURCE_MEM,
        },
        [4] = {
                .start = GPIO1_DIR_BASE,
                .end = GPIO1_DIR_BASE + REGISTER_LENGTH - 1,
                .flags = IORESOURCE_MEM,
        },
};

static void leddevice_release(struct device *dev);

struct platform_device leddevice = {
    .name = "imx6ull-led",      /* 此name必须与驱动中对应 */
    .id = -1,                   /* 表示无设备ID */
    .dev = {
            .release = leddevice_release,
    },
    .num_resources = ARRAY_SIZE(led_resources),
    .resource = led_resources,
};

static void leddevice_release(struct device *dev)
{
        printk("leddevice release!\n");
}

static int __init platform_init(void)
{
        int ret = 0;
        ret = platform_device_register(&leddevice);
        return ret;
}

static void __exit platform_exit(void)
{
        platform_device_unregister(&leddevice);
        return ;
}

module_init(platform_init);
module_exit(platform_exit);
MODULE_AUTHOR("wanglei");
MODULE_LICENSE("GPL");