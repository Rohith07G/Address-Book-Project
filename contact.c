#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "contact.h" // Contact & AddressBook struct definitions, function prototypes
#include <ctype.h>
#include "file.h"   // loadContactsFromFile(), saveContactsToFile()/

int searchByName(AddressBook *addressBook, char *name, int *result)
{
    int i, count = 0;
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].name, name) == 0)  // strcmp == 0 means the strings are identical
        {
            result[count++] = i;    //  store matching index, then move to next slot
        }
    }
    return count;
}
int searchByPhone(AddressBook *addressBook, char *phone, int *result)
{
    int i, count = 0;
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            result[count++] = i;
        }
    }
    return count;  // / number of matches found, used by caller to decide next step
}
int searchByEmail(AddressBook *addressBook, char *email, int *result)
{
    int i, count = 0;
    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].email, email) == 0)
        {
            result[count++] = i;
        }
    }
    return count;
}
void createContact(AddressBook *addressBook)
{
    Contact newContact;
    int attempts = 0;   // tracks invalid attempts
    if(addressBook->contactCount >= 100)
    {
        printf("\nAddress Book Full.\n");
        return;
    }
    while(attempts < 3)   
    {
        printf("\nEnter Name : ");
        scanf(" %[^\n]", newContact.name);
        if(validateName(newContact.name))
            break;
        attempts++;
    }
    if(attempts == 3)
    {
        printf("\nToo many invalid attempts.\n");
        printf("Program terminated.\n");
        exit(0);
    }
    while(attempts < 3)
    {
        printf("Enter Phone : ");
        scanf("%s", newContact.phone);
        if(validatePhone(addressBook, newContact.phone, -1))  // skipIndex = -1 because this is new contact, nothing to exclude from the duplicate check
            break;
        attempts++;
    }
     if(attempts == 3)
     {
        printf("\nToo many invalid attempts.\n");
        printf("Program terminated.\n");
        exit(0);
    }
    while(attempts<3)
    {
        printf("Enter Email : ");
        scanf("%s", newContact.email);

        if(validateEmail(addressBook, newContact.email,-1))
            break;
        attempts++;
    }
     if(attempts == 3)
    {
        printf("\nToo many invalid attempts.\n");
        printf("Program terminated.\n");
        exit(0);
    }
    addressBook->contacts[addressBook->contactCount] = newContact;
    addressBook->contactCount++;
    printf("\nContact Added Successfully.\n");
}
void listContacts(AddressBook *addressBook)
{
    int i;
    if(addressBook->contactCount == 0)
    {
        printf("\nNo Contacts Available.\n");
        return;
    }
    printf("\n------------------------------------------------------------\n");
    printf("%s     %s                %s           %s\n","No","Name","Phone", "Email");
    printf("------------------------------------------------------------\n");
    for(i = 0; i < addressBook->contactCount; i++)
    {
        printf("%d     %s                %s           %s\n\n", i + 1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    // i + 1 so the list is shown starting at 1 instead of 0
    printf("------------------------------------------------------------\n");
}
void searchContact(AddressBook *addressBook)
{
    int choice;
    int result[100] , count = 0;
    char name[50],  phone[11] ,email[100];

    printf("\n===== SEARCH MENU =====\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone\n");
    printf("3. Search by Email\n");
    printf("4. Exit\n");

    printf("Enter Choice : ");
    scanf("%d", &choice);
    
    switch(choice)
    {
        case 1:
            printf("Enter Name : ");
            scanf(" %[^\n]", name);
            count = searchByName(addressBook, name, result);
            break;

        case 2:
            printf("Enter Phone : ");
            scanf("%s", phone);
            count = searchByPhone(addressBook, phone, result);
            break;

        case 3:
            printf("Enter Email : ");
            scanf("%s", email);
            count = searchByEmail(addressBook, email, result);
            break;

        default:
            return;  // / choice 4 (or anything else) just exits the search menu
    }
    if(count == 0)
    {
        printf("\nNo Contact Found.\n");
        return;
    }
    printf("\n---------------------------------------------\n");
    for(int i = 0; i < count; i++)
    {
        Contact *c = &addressBook->contacts[result[i]];
        printf("%d.\n", i + 1);
        printf("Name  : %s\n", c->name);
        printf("Phone : %s\n", c->phone);
        printf("Email : %s\n\n", c->email);
    }
}
void editContact(AddressBook *addressBook)
{
    int choice;
    int result[100];
    int count = 0;
    int select; // holding the actul contact index

    char name[50] , phone[20];
    char email[100];
    printf("\n===== EDIT MENU =====\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("4. Exit\n");

    printf("Enter Choice : ");
    scanf("%d",&choice);
    switch(choice)
    {
        case 1:
        printf("Enter Name : ");
        scanf(" %[^\n]",name);
            count = searchByName(addressBook,name,result);
            break;
            
        case 2:
            printf("Enter Phone : ");
            scanf("%s",phone);
            count = searchByPhone(addressBook,phone,result);
            break;

        case 3:
            printf("Enter Email : ");
            scanf("%s",email);
            count = searchByEmail(addressBook,email,result);
            break;

        default:
            return;
    }
    if(count==0)
    {
        printf("\nContact Not Found.\n");
        return;
    }
    if(count>1)
    {
        /// Same name/phone/email matched more than one contact
        printf("\nMultiple Contacts Found\n");
        for(int i=0;i<count;i++)
        {
            printf("%d. %s %s %s\n",i+1,addressBook->contacts[result[i]].name,addressBook->contacts[result[i]].phone,addressBook->contacts[result[i]].email);
        }
        printf("Select Contact : ");
        scanf("%d",&select);

        if(select<1 || select>count)
        {
            printf("\nInvalid Selection.\n");
            return;
        }
        select = result[select-1];  // / convert the 1-based menu choice into the real contacts[] index
    }
    else
    {
        select = result[0];  
    }

    /// ---- Now that the contact is located, ask what to edit ----
    printf("\nEdit\n");
    printf("1.Name\n");
    printf("2.Phone\n");
    printf("3.Email\n");
    printf("4.Exit\n");

    printf("Choice : ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            while(1)  //  Loop until a valid new name is entered 
            {
                printf("New Name : ");
                scanf(" %[^\n]",name);

                if(validateName(name))
                    break;
            }
            strcpy(addressBook->contacts[select].name,name);
            break;

        case 2:
        while(1)
        {
                printf("New Phone : ");  //  Loop until a valid new phone is entered 
                scanf("%s",phone);
                if(validatePhone(addressBook,phone,select))
                    break;
            }
            strcpy(addressBook->contacts[select].phone,phone);
            break;

        case 3:
            while(1)
            {
                printf("New Email : "); //  Loop until a valid new email is entered 
                scanf("%s",email);
                if(validateEmail(addressBook, email,select))
                    break;
            }
            strcpy(addressBook->contacts[select].email,email);
            break;

        default:
            return;
    }
    printf("\nContact Updated Successfully.\n");
}
void deleteContact(AddressBook *addressBook)
{
    int choice, count=0, select, confirm;
    int result[100];

    char name[50];
    char phone[11];
    char email[100];

    printf("\n===== DELETE MENU =====\n");
    printf("1.Name\n");
    printf("2.Phone\n");
    printf("3.Email\n");
    printf("4.Exit\n");

    printf("Choice : ");
    scanf("%d",&choice);
    switch(choice)
    {
        case 1:
            printf("Enter Name : ");
            scanf(" %[^\n]",name);
            count = searchByName(addressBook,name,result);
            break;

        case 2:
            printf("Enter Phone : ");
            scanf("%s",phone);
            count = searchByPhone(addressBook,phone,result);
            break;

        case 3:
            printf("Enter Email : ");
            scanf("%s",email);
            count = searchByEmail(addressBook,email,result);
            break;

        default:
            return;
    }
    if(count==0)
    {
        printf("\nContact Not Found.\n");
        return;
    }
    if(count>1)
    {
        printf("\nMultiple Contacts Found\n");
        for(int i=0;i<count;i++)
        {
            printf("%d. %s %s %s\n", i+1,addressBook->contacts[result[i]].name, addressBook->contacts[result[i]].phone, addressBook->contacts[result[i]].email);
        }

        printf("Select Contact : ");
        scanf("%d",&select);
        if(select<1 || select>count)
        {
            printf("\nInvalid Selection.\n");
            return;
        }
        select = result[select-1];  /// convert the 1-based menu choice into the real contacts[] index
    }
    else
    {
        select=result[0];
    }
    printf("\nDelete this Contact? (1-Yes 0-No) : ");
    scanf("%d",&confirm);
    if(confirm!=1)   // confirmation to delete or not
    {
        printf("\nDeletion Cancelled.\n");
        return;
    }

    // Shift every contact after "select" one slot to the left,
    // which overwrites the deleted contact and closes the gap
    for(int i=select;i<addressBook->contactCount-1;i++)
    {
        addressBook->contacts[i]=addressBook->contacts[i+1];
    }
    addressBook->contactCount--;  // decrement the count to match the new array content
    printf("\nContact Deleted Successfully.\n");
}
int phoneExists(AddressBook *addressBook, char *phone, int skipIndex)  //unique phone number
{
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(i == skipIndex)
            continue;   // don't compare the contact against its own current phone number

        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
            return 1;
    }
    return 0;
}
int emailExists(AddressBook *addressBook, char email[], int skipIndex)  // unique email
{
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(i == skipIndex)
            continue;

        if(strcmp(addressBook->contacts[i].email, email) == 0)
            return 1;
    }
    return 0;
}
int validateName(char *name)
{
    int len = strlen(name);
    if(len < 4)
    {
        printf("\nError : Name must contain at least 4 characters.\n");
        return 0;
    }
    for(int i = 0; i < len; i++)
    {
        if(!isalpha(name[i]) && name[i] != ' ')  //specific message depending on what kind of error it is
        {
            if(isdigit(name[i]))
                printf("\nError : Numbers are not allowed in name.\n");
            else
                printf("\nError : Symbols are not allowed in name.\n");

            return 0;
        }
    }
    return 1;
}
int validatePhone(AddressBook *addressBook,char *phone,int skipIndex) // phone validations
{
    if(strlen(phone) != 10)
    {
        printf("\nError : Phone number must contain exactly 10 digits.\n");
        return 0;
    }
    for(int i = 0; i < 10; i++)
    {
        if(!isdigit(phone[i]))
        {
            if(isalpha(phone[i]))
                printf("\nError : Alphabets are not allowed in phone number.\n");
            else
                printf("\nError : Symbols are not allowed in phone number.\n");

            return 0;
        }
    }
    if(phone[0] < '6' || phone[0] > '9')
    {
        printf("\nError : First digit must be between 6 and 9.\n");
        return 0;
    }
    if(phoneExists(addressBook, phone, skipIndex))
    {
        printf("\nError : Contact already exists.\n");
        return 0;
    }
    return 1;
}
int validateEmail(AddressBook *addressBook, char *email,int skipIndex)
{
    int at = -1; // index of the '@' character (-1 = not found yet)
    int atCount = 0;
    int dotCount = 0;
    int domainCount = 0;
    int len = strlen(email);

    // First character must be a lowercase alphabet 
           if(email[0] < 'a' || email[0] > 'z')
        {
          printf("\nError: First character must be a lowercase alphabet.\n");
          return 0;
        }

    //  Count '@' and '.' 
    for(int i = 0; i < len; i++)
    {
        if(email[i] == '@')
        {
            atCount++;

            if(at == -1)
                at = i;  // remember the position of the first '@'
        }

        if(email[i] == '.')
            dotCount++;
    }
    // Check exactly one '@' 
    if(atCount != 1)
    {
        printf("\nError: Email must contain exactly one '@'.");
        return 0;
    }
    // Check exactly one '.' 
    if(dotCount != 1)
    {
        printf("\nError: Email must contain exactly one '.'.");
        return 0;
    }
    // Username should not be empty 
    if(at == 0)
    {
        printf("\nError: Username cannot be empty.");
        return 0;
    }
    // Validate username 
    for(int i = 0; i < at; i++)
    {
        /* Uppercase letters are not allowed */
        if(email[i] >= 'A' && email[i] <= 'Z')
        {
            printf("\nError: Uppercase letters are not allowed.");
            return 0;
        }

        // Only lowercase letters and digits are allowed 
        if(!((email[i] >= 'a' && email[i] <= 'z') ||
             (email[i] >= '0' && email[i] <= '9')))
        {
            printf("\nError: Special characters are not allowed before '@'.");
            return 0;
        }
    }
    // Email must end with ".com" 
    if(len < 5 || email[len-4] != '.' || email[len-3] != 'c' || email[len-2] != 'o' || email[len-1] != 'm')
    {
        printf("\nError: Email must end with '.com'.");
        return 0;
    }
    // Count domain characters
    for(int i = at + 1; email[i] != '.'; i++)
    {
        domainCount++;
    }

    // Domain must contain at least 5 characters 
    if(domainCount < 5)
    {
        printf("\nError: Domain must contain at least 5 characters.");
        return 0;
    }

    printf("Email is valid.");

    return 1;
}
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}
void saveAndExit(AddressBook *addressBook){
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}