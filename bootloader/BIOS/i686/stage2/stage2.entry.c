void _cstart()
{
    char* vga_memory = (char*)0xB8000;
    vga_memory[0] = 'A';
}