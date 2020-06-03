


uint32_t
pci_read_config_dword(uint8_t b, uint8_t d, uint8_t f, uint8_t offs)
{
    uint32_t lbus  = (uint32_t)b;
    uint32_t ldev  = (uint32_t)d;
    uint32_t lfunc = (uint32_t)f; 

    uint32_t addr = 0;
    addr |= (1 << 31);
    addr |= (lbus << 16);
    addr |= (ldev << 11);
    addr |= (lfunc << 8);
    addr |= (offs & 0xfc);

    outl(PCI_CONFIG_ADDR, addr); 
    uint32_t result = inl(PCI_CONFIG_DATA);
    
    return (result); 
    
}

PCI_device_id
pci_read_device_id(uint8_t b, uint8_t d, uint8_t f)
{
    PCI_device_id result;

    uint32_t slot = pci_read_config_dword(b, d, f, 0);
    result.vendor_id = (uint16_t)(slot & 0xffff);
    result.device_id = (uint16_t)((slot >> 16) & 0xffff);

    return (result);
}
// NOTE(max): Function assums that a device is present
PCI_class_code
pci_read_class_code(uint8_t b, uint8_t d, uint8_t f)
{
    PCI_class_code result;

    uint32_t slot = pci_read_config_dword(b, d, f, 8);
    result.class_code = (uint8_t)((slot >> 24) & 0xff); 
    result.subclass   = (uint8_t)((slot >> 16) & 0xff); 
    result.prog_if    = (uint8_t)((slot >> 8)  & 0xff); 

    return (result);
}

uint8_t pci_read_header_type(uint8_t b, uint8_t d, uint8_t f)
{
    uint8_t result = (uint8_t)((pci_read_config_dword(b, d, f, 0x0c) >> 16) & 0xff);

    return (result);
}


void
pci_print_function_info(uint8_t bus, uint8_t dev, uint8_t func)
{
    PCI_device_id id = pci_read_device_id(bus, dev, func);
    PCI_class_code class = pci_read_class_code(bus, dev, func);
   
    /*
     * Bus x, device x, function x:
     *   NAME(class): PCI device v_id:d_id
     *   IRQ x
     * */
    printf("  Bus %x, device %x, function %x:\n", bus, dev, func);
    printf("    ");
    if (class.class_code == 0x01)
    {
        if (class.subclass == 0x01)
            printf("IDE controller: ");	
        else
            printf("Mass Storage controller: ");
    }
    else if (class.class_code == 0x02)
    {
        if (class.subclass == 0x00)
            printf("Ethernet controller: ");
        else
            printf("Network controller: ");	
    }
    else if (class.class_code == 0x03)
    {
        if (class.subclass == 0x00 && class.prog_if == 0x00)
            printf("VGA controller: ");	
        else if (class.subclass == 0x00)
            printf("VGA compatible controller: ");	
        else
            printf("Display controller: ");	
    }
    else if (class.class_code == 0x06)
    {
        if (class.subclass == 0x00)
            printf("Host bridge: ");
        else if (class.subclass == 0x01)
	    printf("ISA bridge: ");
        else if (class.subclass == 0x04 || class.subclass == 0x09)
	    printf("PCI-to-PCI bridge: ");
	else
	    printf("Bridge device: ");
    }

    printf("%x : %x\n\n", id.vendor_id, id.device_id);
}

void
pci_print_device_info(uint8_t bus, uint8_t dev)
{
    PCI_device_id id = pci_read_device_id(bus, dev, 0);
    if (id.vendor_id == 0xffff)
        return;

    pci_print_function_info(bus, dev, 0);
    uint8_t hdr_type = pci_read_header_type(bus, dev, 0);
    if (hdr_type & 0x80)
    {
        // NOTE(max): Multifunction device
        for (int func_index = 1; func_index < 8; func_index++)
	{
            PCI_device_id id = pci_read_device_id(bus, dev, func_index);
            if (id.vendor_id != 0xffff)
	    {
                pci_print_function_info(bus, dev, func_index); 
	    }	    
	}
    }
}
void
pci_scan(void)
{
    for (int bus_index = 0; bus_index < 256; bus_index++)
    {
        for (int dev_index = 0; dev_index < 32; dev_index++)
	{
            pci_print_device_info(bus_index, dev_index);
	}	
    }
}





