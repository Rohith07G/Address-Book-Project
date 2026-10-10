#include <string.h>
#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp;
    fp = fopen("contacts.txt", "w");  // "w" = overwrite the file fresh each save
    if(fp == NULL)
    {
        printf("\nUnable to open contacts.txt\n");
        return;
    }
    // Write the count first, so loading knows how many records to expect
    fprintf(fp, "#%d\n", addressBook->contactCount);
    
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        // Comma-separated fields, one contact per line
        fprintf(fp,"%s,%s,%s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    fclose(fp);
    printf("\nContacts saved successfully.\n");
}
void loadContactsFromFile(AddressBook *addressBook)
{
     FILE *fp;
    fp = fopen("contacts.txt", "r");
    if(fp == NULL)
    {
        // No saved file yet which means start with an empty address book
        addressBook->contactCount = 0;  
        return;
    }
    if(fscanf(fp, "#%d\n", &addressBook->contactCount) != 1)
    {
        addressBook->contactCount = 0;
        fclose(fp);
        return;
    }
    for(int i = 0; i < addressBook->contactCount; i++)
        // %[^,] reads up to (not including) the next comma, for name and phone;
        // %[^\n] reads up to the newline for email, since it's the last field on the line
    {
        fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fp);
}


