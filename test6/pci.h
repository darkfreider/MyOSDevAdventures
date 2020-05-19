

#define PCI_CONFIG_ADDR 0xcf8
#define PCI_CONFIG_DATA 0xcfc



struct PCI_device_id
{
    uint16_t vendor_id;
    uint16_t device_id;
};
typedef struct PCI_device_id PCI_device_id;

struct PCI_class_code
{
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
};
typedef struct PCI_class_code PCI_class_code;

void pci_scan(void);


















