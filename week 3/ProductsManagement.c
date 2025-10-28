#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct // structure for Product
{
    int id;
    char *name;
    float price;
    long int quantity;
} Product;

//function for input of product details for initial products
void inputProductDetails(Product *products, int numberOfProducts)
{
    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        printf("Enter details for product %d:\n", iterate + 1);

        printf("Product ID: ");
        scanf("%d", &products[iterate].id);
        //check for valid ID
        if (products[iterate].id < 1 || products[iterate].id > 10000)
        {
            printf("Id must be between 1 and 10000.");
            free(products); // free the products array if memory allocation fails
            exit(1);
        }

        printf("Product Name: ");
        products[iterate].name = (char *)malloc(50 * sizeof(char)); //allocate memory for name
        if (products[iterate].name == NULL)
        {
            printf("Memory allocation for name failed.\n");
            free(products); // free the products array if memory allocation fails
            exit(1);
        }
        scanf("%49s", products[iterate].name);
        if (strlen(products[iterate].name) < 1 || strlen(products[iterate].name) > 50)
        {
            printf("Name length must be between 1 and 50 characters.");
            free(products[iterate].name); // free the name if invalid
            free(products); //also free the products array
            exit(1);
        }

        printf("Product Price: ");
        scanf("%f", &products[iterate].price);
        if (products[iterate].price < 0.00 || products[iterate].price > 100000.00)
        {
            printf("Price must be between 0 and 100000.");
            free(products[iterate].name); // free the name if invalid
            free(products); //also free the products array
            exit(1);
        }

        printf("Product Quantity: ");
        scanf("%ld", &products[iterate].quantity);
        if (products[iterate].quantity < 0 || products[iterate].quantity > 1000000)
        {
            printf("Quantity must be between 0 and 1000000.");
            free(products[iterate].name); // free the name if invalid
            free(products); //also free the products array
            exit(1);
        }
        
    }
}

// function for adding a new product
Product *addNewProduct(Product *products, int *numberOfProducts)
{
    *numberOfProducts += 1; // increase the count of products
    Product *inValid = NULL; // pointer to return in case of invalid input
    Product *newProducts = (Product *)realloc(products, (*numberOfProducts) * sizeof(Product)); // pointer to hold new reallocated memory
    if (newProducts == NULL) {
        printf("Memory reallocation failed.\n");
        *numberOfProducts -= 1; //decrese the count back if reallocation fails
        return products;// return original pointer
    }

    printf("Enter new product details: \n");
    int newIndex = *numberOfProducts - 1; // adding new product at the end of the array

    printf("Product ID: ");
    scanf("%d", &newProducts[newIndex].id);
    if (newProducts[newIndex].id < 1 || newProducts[newIndex].id > 10000)
    {
        printf("Id must be between 1 and 10000.");
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }

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
        return inValid; // return pointer after downgrading back to original array
    }

    printf("Product Price: ");
    scanf("%f", &newProducts[newIndex].price);
    if (newProducts[newIndex].price < 0.00 || newProducts[newIndex].price > 100000.00)
    {
        printf("Price must be between 0 and 100000.");
        free(newProducts[newIndex].name);
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }

    printf("Product Quantity: ");
    scanf("%ld", &newProducts[newIndex].quantity);
    if (newProducts[newIndex].quantity < 0 || newProducts[newIndex].quantity > 1000000)
    {
        printf("Quantity must be between 0 and 1000000.");
        free(newProducts[newIndex].name);
        *numberOfProducts -= 1;
        inValid = (Product *)realloc(newProducts, (*numberOfProducts) * sizeof(Product));
        return inValid; // return pointer after downgrading back to original array
    }

    printf("New product added successfully.\n");
    return newProducts; // if nothing goes wrong return the new pointer
}

//function for the displaying all products
void viewAllProducts(Product *products, int numberOfProducts)
{
    printf("========= PRODUCT LIST =========  \n");

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
    }
}

//function for updating product quantity for particular product
void updateProductQuantity(Product *product, int numberOfProducts)
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
void searchProductByID(Product *products, int numberOfProducts)
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
void searchProductByName(Product *products, int numberOfProducts)
{
    char *searchName = (char *)malloc(50 * sizeof(char)); // allocate memory for search name
    if (searchName == NULL)
    {
        printf("Memory allocation for search name failed.\n");
        return;
    }
    printf("Enter name to search (partial allowed): ");
    scanf("%s", searchName);

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (strstr(products[iterate].name, searchName) != NULL) //using substring function to allow partial match
        {
           printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
           return; 
        }
    }
    printf("Product with name '%s' not found.\n", searchName);
    free(searchName); //free allocated memory for search name
}

//function for searching product by price range
void searchProductByPriceRange(Product *products, int numberOfProducts)
{
    float minPrice, maxPrice;
    bool foundFlag = false;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    for (int iterate = 0; iterate < numberOfProducts; iterate++)
    {
        if (products[iterate].price >= minPrice && products[iterate].price <= maxPrice)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %ld\n", products[iterate].id, products[iterate].name, products[iterate].price, products[iterate].quantity);
            foundFlag =true;
        }
    }
    if (!foundFlag){
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
            // if product is not in last position shift all products after it to left by one position
            if (iterate < *numberOfProducts - 1)
            {
            for (int shiftIndex = iterate; shiftIndex < *numberOfProducts - 1; shiftIndex++)
            {
                free(products[shiftIndex].name);//free the name memory of the current position before overwriting
                products[shiftIndex].id = products[shiftIndex + 1].id;
                products[shiftIndex].name = strdup(products[shiftIndex + 1].name); //duplicate the string to avoid dangling pointer
                products[shiftIndex].price = products[shiftIndex + 1].price;
                products[shiftIndex].quantity = products[shiftIndex + 1].quantity;
            }
            }
            //free the name memory if product to be deleted is in last position or after shifting last being duplicated
            free(products[*numberOfProducts - 1].name);
            *numberOfProducts -= 1; // decrese the count of products
            //reallocate memory to shrink the array size
            Product *newProducts = (Product *)realloc(products, (*numberOfProducts) * sizeof(Product));
            if(newProducts == NULL && *numberOfProducts > 0)
            {
                printf("Memory reallocation fdailed after deletion.\n");
                return products; // if reallocation fails return original pointer, but deleted is already done, just size is still larger
            }
            printf("Product with ID %d deleted successfully.\n", deleteId);
            return newProducts; // return the new pointer after deletion if nothing goes wrong
        }
    }
    printf("Product with ID %d not found.\n", deleteId);
    return products; // return original pointer if product to delete not found
}


int main() {
    int numberOfProducts;
    bool exitFlag = true;
    int menuOption;
    printf("Enter initial number of products: ");
    scanf("%d", &numberOfProducts);

    //validate number of products
    if (numberOfProducts < 1 || numberOfProducts > 100) {
        printf("The range of products must be between 1 and 100.\n");
        return 1;
    }

    //allocate memory for products
    Product *products = (Product *)calloc(numberOfProducts, sizeof(Product));
    if (products == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    inputProductDetails(products, numberOfProducts);
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
            products = addNewProduct(products, &numberOfProducts);
            break;
        case 2:
            viewAllProducts(products, numberOfProducts);
            break;
        case 3:
            updateProductQuantity(products, numberOfProducts);
            break;
        case 4:
            searchProductByID(products, numberOfProducts);
            break;
        case 5:
            searchProductByName(products, numberOfProducts);
            break;
        case 6:
            searchProductByPriceRange(products, numberOfProducts);
            break;
        case 7:
            products = deleteProduct(products, &numberOfProducts);
            break;
        default:
            exitFlag = false;
            break;
        }
    } while (exitFlag);

    //freeing allocated memory before exiting the program
    for (int iterate = 0; iterate < numberOfProducts; iterate++){
        free(products[iterate].name);
    }
    free(products);
    return 0;
}