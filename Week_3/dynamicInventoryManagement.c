#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

typedef struct
{
    int id;
    char *name;
    float price;
    long int quantity;
} Product;

bool hasDuplicateID(Product *products, int numberOfProducts, int productId)
{
    bool isDuplicate = false;
    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (products[iterate].id == productId){
            isDuplicate = true;
            break;
        }
    }
    return isDuplicate;
}

//function for input of product details for initial products
void enterDetails(Product *products, int numberOfProducts)
{
    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        int productId;
        printf("Enter details for product %d:\n", iterate + 1);

        printf("Product ID: ");
        //check for valid ID
        if (scanf("%d", &productId) != 1 || productId < 1 || productId > 10000)
        {
            while (getchar() != '\n');
            printf("Id must be number between 1 and 10000.");
            free(products);
            exit(1);
        }
        if (hasDuplicateID(products, numberOfProducts, productId))
        {
            printf("This ID is already present. Please provide other unique ID.");
            free(products);
            exit(1);
        }

        products[iterate].id = productId;

        printf("Product Name: ");
        products[iterate].name = (char *)malloc(50 * sizeof(char));
        if (products[iterate].name == NULL)
        {
            printf("Memory allocation for name failed.\n");
            free(products);
            exit(1);
        }
        scanf("%49s", products[iterate].name);
        if (strlen(products[iterate].name) < 1 || strlen(products[iterate].name) > 50)
        {
            printf("Name length must be between 1 and 50 characters.");
            free(products[iterate].name);
            free(products);
            exit(1);
        }

        float productPrice;
        printf("Product Price: ");
        // validate input
        if (scanf("%f", &productPrice) != 1 || productPrice < 0.00 || productPrice > 100000.00)
        {
            while (getchar() != '\n');
            printf("Price must be decimal value between 0 and 100000.");
            free(products[iterate].name);
            free(products);
            exit(1);
        }
        products[iterate].price = productPrice;

        long int productQuantity;
        printf("Product Quantity: ");
        
        if (scanf("%ld", &productQuantity) != 1 || productQuantity < 0 || productQuantity > 1000000)
        {
            while (getchar() != '\n');
            printf("Quantity must be between 0 and 1000000.");
            free(products[iterate].name);
            free(products);
            exit(1);
        }
        products[iterate].quantity = productQuantity;       
    }
}

// function for adding a new product
Product *addProduct(Product *products, int *numberOfProducts)
{
    *numberOfProducts += 1;
    Product *inValid = NULL;
    Product *newProducts = (Product *)realloc(products, (*numberOfProducts) * sizeof(Product));
    if (newProducts == NULL) {
        printf("Memory reallocation failed.\n");
        *numberOfProducts -= 1; //decrese the count back if reallocation fails
        return products;// return original pointer
    }

    printf("Enter new product details: \n");
    int newIndex = *numberOfProducts - 1;

    int productId;
    printf("Product ID: ");
    
    if (scanf("%d", &productId) != 1 || productId < 1 || productId > 10000)
    {
        while (getchar() != '\n');
        printf("Id must be number between 1 and 10000.");
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }
    if (hasDuplicateID(newProducts, newIndex, productId))
    {
        printf("This ID is already present. Please provide other unique ID.");
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid;
    }
    
    newProducts[newIndex].id = productId;

    printf("Product Name: ");
    newProducts[newIndex].name = (char *)malloc(50 * sizeof(char));
    if (newProducts[newIndex].name == NULL)
    {
        printf("Memory allocation for name failed.\n");
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }
    scanf("%49s", newProducts[newIndex].name);
    if (strlen(newProducts[newIndex].name) < 1 || strlen(newProducts[newIndex].name) > 50)
    {
        printf("Name length must be between 1 and 50 characters.");
        free(newProducts[newIndex].name);
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid;
    }

    float productPrice;
    printf("Product Price: ");

    if (scanf("%f", &productPrice) != 1 || productPrice < 0.00 || productPrice > 100000.00)
    {
        while (getchar() != '\n');
        printf("Price must be decimal number between 0 and 100000.");
        free(newProducts[newIndex].name);
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }
    newProducts[newIndex].price = productPrice;

    long int productQuantity;
    printf("Product Quantity: ");
    
    if (scanf("%ld", &productQuantity) != 1 || productQuantity < 0 || productQuantity > 1000000)
    {
        while (getchar() != '\n');
        printf("Quantity must be between 0 and 1000000.");
        free(newProducts[newIndex].name);
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }
    newProducts[newIndex].quantity = productQuantity;

    printf("New product added successfully.\n");
    return newProducts; // if nothing goes wrong return the new pointer
}

//function for the displaying all products
void viewProducts(Product *products, int numberOfProducts)
{
    printf("========= PRODUCT LIST =========  \n");

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
    }
}

//function for updating product quantity for particular product
void updateQuantity(Product *product, int numberOfProducts)
{
    int searchId;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &searchId);

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (product[iterate].id == searchId)
        {
            long int newQuantity;
            printf("Enter new Quantity: ");
            scanf("%ld", &newQuantity);
            if (newQuantity < 0 || newQuantity > 1000000)
            {
                printf("Quantity must be between 0 and 1000000.");
                return;
            }
            product[iterate].quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            return;
        }
    }
    printf("Product with ID %d not found.\n", searchId);
}

//function for searching product by ID
void searchByID(Product *products, int numberOfProducts)
{
    int searchId;
    printf("Enter Product ID to search: ");
    scanf("%d", &searchId);

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (products[iterate].id == searchId)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
            return;
        }
    }
    printf("Product with ID %d not found.\n", searchId);
}

//functyion for searching product by Name
void searchByName(Product *products, int numberOfProducts)
{
    char *searchName = (char *)malloc(50 * sizeof(char)); 
    if (searchName == NULL)
    {
        printf("Memory allocation for search name failed.\n");
        return;
    }
    printf("Enter name to search (partial allowed): ");
    scanf("%s", searchName);

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (strstr(products[iterate].name, searchName) != NULL)
        {
           printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
           free(searchName);
           return; 
        }
    }
    printf("Product with name '%s' not found.\n", searchName);
    free(searchName); //free allocated memory for search name
}

//function for searching product by price range
void searchByPrice(Product *products, int numberOfProducts)
{
    float minPrice, maxPrice;
    bool found = false;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (products[iterate].price >= minPrice && products[iterate].price <= maxPrice)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
            found =true;
        }
    }
    if (!found){
        printf("No products found in the price range %.2f - %.2f.\n", minPrice, maxPrice);
    }
}

//function for deleting a product by ID
Product *deleteProduct(Product *products, int *numberOfProducts)
{
    int deleteId;
    printf("Enter Product ID to delete: ");
    scanf("%d", &deleteId);

    for (int iterate = 0; iterate < *numberOfProducts; iterate++)
    {
        if(products[iterate].id == deleteId)
        {
            if (iterate < *numberOfProducts - 1)
            {
            for (int shiftIndex = iterate; shiftIndex < *numberOfProducts - 1; shiftIndex++)
            {
                free(products[shiftIndex].name);
                products[shiftIndex].id = products[shiftIndex + 1].id;
                products[shiftIndex].name = strdup(products[shiftIndex + 1].name);
                products[shiftIndex].price = products[shiftIndex + 1].price;
                products[shiftIndex].quantity = products[shiftIndex + 1].quantity;
            }
            }
        
            free(products[*numberOfProducts - 1].name);
            *numberOfProducts -= 1; 
            
            Product *newProducts = (Product *)realloc(products, (*numberOfProducts) * sizeof(Product));
            if(newProducts == NULL && *numberOfProducts > 0)
            {
                printf("Memory reallocation fdailed after deletion.\n");
                return products;
            }
            printf("Product with ID %d deleted successfully.\n", deleteId);
            return newProducts;
        }
    }
    printf("Product with ID %d not found.\n", deleteId);
    return products;
}


int main() {
    int numberOfProducts;
    bool exitFlag = true;
    int menuOption;
    printf("Enter initial number of products: ");
    scanf("%d", &numberOfProducts);


    if (numberOfProducts < 1 || numberOfProducts > 100) {
        printf("The range of products must be between 1 and 100.\n");
        return 1;
    }

    
    Product *products = (Product *)calloc(numberOfProducts, sizeof(Product));
    if (products == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    enterDetails(products, numberOfProducts);
    printf("\n");
    printf("========= INVENTORY MENU =========\n");
    do {
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &menuOption);
        switch (menuOption)
        {
        case 1:
            products = addProduct(products, &numberOfProducts);
            break;
        case 2:
            viewProducts(products, numberOfProducts);
            break;
        case 3:
            updateQuantity(products, numberOfProducts);
            break;
        case 4:
            searchByID(products, numberOfProducts);
            break;
        case 5:
            searchByName(products, numberOfProducts);
            break;
        case 6:
            searchByPrice(products, numberOfProducts);
            break;
        case 7:
            products = deleteProduct(products, &numberOfProducts);
            break;
        default:
            exitFlag = false;
            break;
        }
    } while (exitFlag);

    for (int iterate = 0; iterate < numberOfProducts; iterate++){
        free(products[iterate].name);
    }
    free(products);
    return 0;
}