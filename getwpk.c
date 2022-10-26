#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <linux/platform_device.h>

// Module information. GPL license needed for platform_device.h
MODULE_DESCRIPTION("Write the WPK in the MSDM table in the ACPI to a file.");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");

// Get WPK.
static ssize_t wpk_show(struct device *dev, struct device_attribute *attr, char *buf) {
	// Pointer to the ACPI.
	struct acpi_table_header *table;

	// Get the ACPI.
	acpi_status status = acpi_get_table(ACPI_SIG_MSDM, 0, &table);

	// If getting the ACPI table was a failure, handle.
	if (ACPI_FAILURE(status)) {
		pr_err("Failed to get MSDM table: %s\n", acpi_format_exception(status));
		return -ENODEV;
	}

	// Get the MSDM table.
	struct acpi_table_msdm *msdm = (struct acpi_table_msdm *)table;

	// Get the MSDM table, as bytes.
	u8 *body = (u8 *) (table) + sizeof(struct acpi_table_header);

	// Get the length of such.
	u32 body_len = msdm->header.length - sizeof(struct acpi_table_header);

	// Counter.
	int i;

	// Search for WPK beyond the header.
	for (i = 0; i < body_len; i++) {
		// Break once alphanumeric character is found; indicative of WPK.
		if (isalnum(body[i])) break;
	}

	// Return error if no WPK found.
	if (i == body_len) return -ENODEV;

	// Get WPK.
	int wpk_len = snprintf(buf, PAGE_SIZE, "%.*s\n", body_len - i, body + i);

	// Release ACPI.
	acpi_put_table(table);

	// Return WPK length.
	return wpk_len;
}

static DEVICE_ATTR(wpk, S_IRUSR, wpk_show, NULL);

static struct platform_device *wpk_dev;

static int __init getwpk_init(void) {
	// Register platform device.
	wpk_dev = platform_device_register_simple("getwpk", -1, NULL, 0);

	// Handle unsuccessful registration.
	if (IS_ERR(wpk_dev)) {
		// Store error.
		long err = PTR_ERR(wpk_dev);

		pr_err("Failed to register getwpk platform device.\n");

		// Set device pointer to NULL.
		wpk_dev = NULL;

		// Return error.
		return err;
	}

	// Create attribute file for platform device.
	int status = device_create_file(&wpk_dev->dev, &dev_attr_wpk);

	// Handle unsuccessful file creation.
	if (status) {
		pr_err("Failed to create sysfs attribute file for getwpk platform device.\n");

		// Return error.
		return status;
	}

	return 0;
}

static void __exit getwpk_exit(void) {
	// Release.
	if (wpk_dev) platform_device_unregister(wpk_dev);
}

module_init(getwpk_init);
module_exit(getwpk_exit);