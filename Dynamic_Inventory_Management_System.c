#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LENGTH 50

#define MIN_PRODUCT_QUANTITY 0
#define MAX_PRODUCT_QUANTITY 1000000

#define MIN_PRODUCT_PRICE 0
#define MAX_PRODUCT_PRICE 100000

#define MIN_PRODUCT_ID 1
#define MAX_PRODUCT_ID 10000

#define MIN_NUMBER_OF_PRODUCTS 1
#define MAX_NUMBER_OF_PRODUCTS 100

struct Product
{
	int productId;
	char productName[NAME_LENGTH];
	float productPrice;
	int productQuantity;
};

void addProductDetails(struct Product *product)
{
	printf("Product ID: ");
	if (scanf("%d", & product->productId) != 1)
	{
		printf("Product ID should be numeric value\n");
		return;
	}
	
	if (product->productId < MIN_PRODUCT_ID || product->productId > MAX_PRODUCT_ID)
	{
		printf("Product Quantity should be in between 0 and 10000");
		return;
	}
	
	printf("Product Name: ");
	scanf("%49s", product->productName);
	
	printf("Product Price: ");
	
	if (scanf("%f", & product->productPrice) != 1)
	{
		printf("Product Price should be numeric value\n");
		return;
	}
	
	if (product->productPrice < MIN_PRODUCT_PRICE || product->productPrice > MAX_PRODUCT_PRICE)
	{
		printf("Product Quantity should be in between 0 and 100000");
		return;
	}
	
	printf("Product Quantity: ");
	
	if (scanf("%d", & product->productQuantity) != 1)
	{
		printf("Product Quantity should be numeric value\n");
		return;
	}
	
	if (product->productQuantity < MIN_PRODUCT_QUANTITY || product->productQuantity > MAX_PRODUCT_QUANTITY)
	{
		printf("Product Quantity should be in between 0 and 1000000");
		return;
	}
}

void addProducts(struct Product *products , int numberOfProducts)
{
	int index;
	struct Product *temporaryPointer = products;
	
	for(index = 0; index < numberOfProducts; index++, temporaryPointer++)
	{
		printf("\nEnter details for product: %d\n", (index + 1));
		addProductDetails(temporaryPointer);
	} 
}

void addNewProduct(struct Product **products , int *numberOfProducts)
{
	printf("\nEnter new product details: \n");
	
	int newCountOfProducts = *numberOfProducts + 1;
	
	struct Product *temporaryProducts = realloc(*products , newCountOfProducts * sizeof(struct Product));
	
	if(temporaryProducts == NULL)
	{
		printf("Memory allocation failed! \n");
		return;
	}
	
	*products = temporaryProducts;
	
	addProductDetails((*products) + *numberOfProducts);
	*numberOfProducts = newCountOfProducts;
	
	printf("\nProduct added successfully \n");
}

void displayProductDetails(struct Product *products , int numberOfProducts)
{
	int index; 
	struct Product *temporaryPointer = products;
	
	printf("\n===================== PRODUCT LIST ===================== \n");
	
	for(index = 0; index < numberOfProducts; index++, temporaryPointer++)
	{
		printf("\nProduct ID: %d | Name: %s | Price: %0.2f | Quantity: %d", temporaryPointer->productId , temporaryPointer->productName , temporaryPointer->productPrice , temporaryPointer->productQuantity);
	}
	
	printf("\n");
}

void updateProductQuantity(struct Product *products , int numberOfProducts)
{
	int updateProductID;
	int index;
	int updatedQuantity;
	int productExist = 0;
	struct Product *temporaryPointer = products;
	
	printf("\nEnter Product ID to update quantity: ");
	if (scanf("%d", & updateProductID) != 1)
	{
		printf("Product ID should be numeric value");
		return;
	}
	
	if (updateProductID < MIN_PRODUCT_ID || updateProductID > MAX_PRODUCT_ID)
	{
		printf("Product Quantity should be in between 0 and 10000");
		return;
	}
	
	for(index = 0; index < numberOfProducts; index++, temporaryPointer++)
	{
		if(updateProductID == temporaryPointer->productId)
		{
			printf("Enter new Quantity: ");
			if (scanf("%d", &updatedQuantity) != 1)
			{
				printf("Product Quantity should be numeric value\n");
				return;
			}
			
			if (updatedQuantity < MIN_PRODUCT_QUANTITY || updatedQuantity > MAX_PRODUCT_QUANTITY)
			{
				printf("Product Quantity should be in between 0 and 1000000");
				return;
			}
			
			temporaryPointer->productQuantity = updatedQuantity;
			printf("\nQuantity updated successfully!\n");
			
			productExist = 1;
			return;
		}
	}
	
	if (productExist == 0)
	{
		printf("Product not Found\n");
		return;	
	}	
}

void searchProductById(struct Product *products , int numberOfProducts)
{
	int searchProductID;
	int index;
	int productExist = 0;
	struct Product *temporaryPointer = products;
	
	printf("\nEnter Product ID to search: ");
	if (scanf("%d", & searchProductID) != 1)
	{
		printf("Product ID should be numeric value");
		return;
	}
	
	if (searchProductID < MIN_PRODUCT_ID || searchProductID > MAX_PRODUCT_ID)
	{
		printf("Product Quantity should be in between 0 and 10000");
		return;
	}
	
	for(index = 0; index < numberOfProducts; index++, temporaryPointer++)
	{
		if(searchProductID == temporaryPointer->productId)
		{
			printf("Product Found:\n");
			printf("Product ID: %d | Name: %s | Price: %0.2f | Quantity: %d\n", temporaryPointer->productId, temporaryPointer->productName, temporaryPointer->productPrice, temporaryPointer->productQuantity);
			
			productExist = 1;
			return;
		}
	}
	
	if (productExist == 0)
	{
		printf("Product not Found\n");
		return;	
	}	
}

void searchProductByPriceRange(struct Product *products , int numberOfProducts)
{
	float minimumPrice;
	float maximumPrice;
	int index;
	int productExist = 0;
	struct Product *temporaryPointer = products;
	
	printf("\nEnter minimum price: ");
	scanf("%f", &minimumPrice);
	
	printf("Enter maximum price: ");
	scanf("%f", &maximumPrice);
	
	printf("\nProducts in price range:\n");
	
	for(index = 0; index < numberOfProducts; index++, temporaryPointer++)
	{
		if(temporaryPointer->productPrice >= minimumPrice && temporaryPointer->productPrice <= maximumPrice)
		{
			printf("Product ID: %d | Name: %s | Price: %0.2f | Quantity: %d\n", temporaryPointer->productId, temporaryPointer->productName, temporaryPointer->productPrice, temporaryPointer->productQuantity);
			productExist = 1;
		}
	}
	
	if (productExist == 0)
	{
		printf("Product not Found\n");
		return;	
	}
}

void deleteProduct(struct Product **products , int *numberOfProducts)
{
	int deleteProductID;
	int productExist = 0;
	
	printf("\nEnter Product ID to delete: ");
	
	if (scanf("%d", & deleteProductID) != 1)
	{
		printf("Product ID should be numeric value");
		return;
	}
	
	if (deleteProductID < MIN_PRODUCT_ID || deleteProductID > MAX_PRODUCT_ID)
	{
		printf("Product Quantity should be in between 0 and 10000");
		return;
	}
	
	int index;
	int shiftIndex;
	struct Product *temporaryPointer = *products;
	
	for(index = 0; index < *numberOfProducts; index++, temporaryPointer++)
	{
		if(deleteProductID == temporaryPointer->productId)
		{
			shiftIndex = index;
			
			for (shiftIndex = index; shiftIndex < *numberOfProducts - 1; shiftIndex++)
            {
                (*products)[shiftIndex] = (*products)[shiftIndex + 1];
            }

            (*numberOfProducts)--;

            *products = realloc(*products, (*numberOfProducts) * sizeof(struct Product));
            printf("Product deleted successfully!\n");
            
            productExist = 1;
            return;
		}
	}
	
	if (productExist == 0)
	{
		printf("Product not Found\n");
		return;	
	}
}

void toLowerCase(char *searchProductName)
{
	while(*searchProductName)
	{
		*searchProductName = tolower(*searchProductName);
		searchProductName++;	
	}	
}

void searchProductByName(struct Product *products , int numberOfProducts)
{
	char searchProductName[NAME_LENGTH];
	int productExist = 0;
	
	printf("\nEnter name to search (partial allowed): ");
	scanf("%49s", searchProductName);
	toLowerCase(searchProductName);
	
	struct Product *temporaryPointer = products;
	
	int index;
	for(index = 0; index < numberOfProducts; index++, temporaryPointer++)
	{
		char productNameLower[NAME_LENGTH];
        strcpy(productNameLower, temporaryPointer->productName);
        toLowerCase(productNameLower);
		
		if (strstr(productNameLower , searchProductName))
		{
			printf("Product Found:\n");
			printf("Product ID: %d | Name: %s | Price: %0.2f | Quantity: %d\n", temporaryPointer->productId, temporaryPointer->productName, temporaryPointer->productPrice, temporaryPointer->productQuantity);
			productExist = 1;
		}
	}
	
	if (productExist == 0)
	{
		printf("Product not Found\n");
		return;	
	}
}

int main()
{
	int numberOfProducts = 0;
	
	printf("Enter intial number of products: ");
	
	if (scanf("%d", &numberOfProducts) != 1)
	{
		printf("Number of Products should be a numric value");
		return 1;	
	}	
	
	if (numberOfProducts < MIN_NUMBER_OF_PRODUCTS  || numberOfProducts > MAX_NUMBER_OF_PRODUCTS)
	{
		printf("Number of Products should in between 1 and 100");
		return 1;
	}
	
	struct Product *products = calloc(numberOfProducts , sizeof(struct Product));
	
	if(products == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}
	
	addProducts(products , numberOfProducts);

	int choice;
	
	while (1)
	{
		printf("\n===================== INVENTORY MENU =====================\n");
		printf("\n 1. Add New Product \n");
		printf("\n 2. View All Products \n");
		printf("\n 3. Update Quantity \n");
		printf("\n 4. Search Product by ID \n");
		printf("\n 5. Search Product by Name \n");
		printf("\n 6. Search Product by Price Range \n");
		printf("\n 7. Delete Product \n");
		printf("\n 8. Exit \n");
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		
		switch(choice)
		{
			case 1:
				addNewProduct(&products , &numberOfProducts);
				break;
			
			case 2:
				displayProductDetails(products , numberOfProducts);
				break;
				
			case 3:
				updateProductQuantity(products , numberOfProducts);
				break;
				
			case 4:
				searchProductById(products , numberOfProducts);
				break;
				
			case 5:
				searchProductByName(products , numberOfProducts);
				break;
				
			case 6:
				searchProductByPriceRange(products , numberOfProducts);
				break;
				
			case 7:
				deleteProduct(&products , &numberOfProducts);
				break;
				
			case 8:
				free(products);
				printf("Memory released successfully. Exiting program...\n");
				return 0;
				
			default:
				printf("Invalid Choice! Try Again\n");
		}
	}
	
	return 0;
}
