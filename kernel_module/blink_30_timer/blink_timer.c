#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO_ADDR_BASE			0X44E07000
#define ADDR_SIZE			0X1000
#define GPIO_SETDATAOUT_OFFSET		0X194
#define GPIO_CLEARDATAOUT_OFFSET	0X190
#define GPIO_OE_OFFSET			0X134
#define LED				~(1 << 30)
#define DATA_OUT			(1 << 30)

void __iomem *base_addr;
struct timer_list my_timer;
unsigned int count = 0;

static void timer_function(unsigned long data) {
	if ((count % 2) == 0)
		writel_relaxed(DATA_OUT, base_addr + GPIO_SETDATAOUT_OFFSET);
	else
		writel_relaxed(DATA_OUT, base_addr + GPIO_CLEARDATAOUT_OFFSET);

	count++;
	mod_timer(&my_timer, jiffies + HZ);
}

int init_module(void)
{
	uint32_t reg_data = 0;

	printk(KERN_EMERG "Hello");

	base_addr = ioremap(GPIO_ADDR_BASE, ADDR_SIZE);

	reg_data = readl_relaxed(base_addr + GPIO_OE_OFFSET);
	reg_data &= LED;
	writel_relaxed(reg_data, base_addr + GPIO_OE_OFFSET);

	init_timer(&my_timer);
	my_timer.expires = jiffies + HZ;
	my_timer.function = timer_function;
	my_timer.data = 0;
	add_timer(&my_timer);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_EMERG "Goodbye");

	del_timer(&my_timer);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phu Luu An");
MODULE_DESCRIPTION("Hello world kernel module");
