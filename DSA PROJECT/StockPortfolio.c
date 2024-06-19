#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "StockMarket.h"

#define FALSE 0
#define TRUE 1
 
User* createUser(const char name[], const double riskThreshold) {
 
    User* newUser = (User*)malloc(sizeof(User));
 
    memset(newUser, 0, sizeof(User));
    newUser->risk_threshold = riskThreshold;
 
    strncpy(newUser->name, name, sizeof(newUser->name));
    newUser->name[sizeof(newUser->name) - 1] = '\0';
   
    return newUser;
}
 
User* findUser(User* users, char name[]) {
    User* currentUser = users;
    while (currentUser != NULL) {
        if (strcmp(currentUser->name, name) == 0) {
            return currentUser;
        }
        currentUser = currentUser->next;
    }
    return NULL;
}
 
User* addUser(User* users, char name[]) {
    User* existingUser = findUser(users, name);
    if (existingUser != NULL) {
        printf("\n User '%s' already exists.\n", name);
        return users;
    }
 
    double riskThreshold = 700;
    User* newUser = createUser(name, riskThreshold);
    newUser->next = users;
    return newUser;
}
 
PortfolioList* createPortfolio(Stock stock) {
    PortfolioList* newNode = (PortfolioList*)malloc(sizeof(PortfolioList));
 
   
    strncpy(newNode->stock.symbol, stock.symbol, sizeof(newNode->stock.symbol) - 1);
    newNode->stock.symbol[sizeof(newNode->stock.symbol) - 1] = '\0';
 
    strncpy(newNode->stock.name, stock.name, sizeof(newNode->stock.name) - 1);
    newNode->stock.name[sizeof(newNode->stock.name) - 1] = '\0';
 
    newNode->stock.purchase_price = stock.purchase_price;
    newNode->stock.quantity = stock.quantity;
 
    newNode->stock.num_prices = stock.num_prices;
 
    newNode->stock.historical_prices = (double*)malloc(sizeof(double) * stock.num_prices);
    memcpy(newNode->stock.historical_prices, stock.historical_prices, sizeof(double) * stock.num_prices);
 
    newNode->left = newNode->right = NULL;
 
    return newNode;
}
 
 
PortfolioList* insert(PortfolioList* root, Stock stock) {
 
    if (root == NULL) {
        return createPortfolio(stock);
    }
 
    int cmp = strcmp(stock.symbol, root->stock.symbol);
    if (cmp < 0) {
        root->left = insert(root->left, stock);
    }
    else if (cmp > 0) {
        root->right = insert(root->right, stock);
    }
    printf("\n Inserting stock %s with historical prices: ", stock.symbol);
    for (int i = 0; i < stock.num_prices; ++i) {
        printf("%lf ", stock.historical_prices[i]);
    }
    printf("\n");
    return root;
}
 
Stock* find(PortfolioList* root, char* symbol) {
    if (root == NULL) {
        return NULL;
    }
 
    int cmp = strcmp(symbol, root->stock.symbol);
    if (cmp == 0) {
        return &(root->stock);
    }
    else if (cmp < 0) {
        return find(root->left, symbol);
    }
    else {
        return find(root->right, symbol);
    }
}
 
 
 
double calculatePortfolioValue(PortfolioList* root) {
    if (root == NULL) {
        return 0.0;
    }
 
    double currentValue = root->stock.purchase_price * root->stock.quantity;
    double leftValue = calculatePortfolioValue(root->left);
    double rightValue = calculatePortfolioValue(root->right);
 
    return currentValue + leftValue + rightValue;
}
 
 
 
void UpdateUserPortfolio(Stock stock, User* user) {
 
    user->initialInvestment += stock.purchase_price * stock.quantity;
    user->portfolio_value = calculatePortfolioValue(user->portfolio);
 
    Stock* currentHighest = find(user->portfolio, user->highest_priced_stock);
    if (currentHighest == NULL || stock.purchase_price > currentHighest->purchase_price) {
        strncpy(user->highest_priced_stock, stock.symbol, sizeof(user->highest_priced_stock) - 1);
        user->highest_priced_stock[sizeof(user->highest_priced_stock) - 1] = '\0';
        user->portfolio_value = calculatePortfolioValue(user->portfolio);
    }
 
}
 
 
void displayAllUsersPortfolio(User* users) {
    printf("\nPortfolio of all users:\n");
    printf("%-20s%-20s%-20s\n", "User", "Portfolio Value", "Highest-Priced Stock");
    User* currentUser = users;
 
    while (currentUser != NULL) {
        double portfolioValue = calculatePortfolioValue(currentUser->portfolio);
 
 
        double highestPrice = 0.0;
        char highestPriceStock[10] = "N/A";
 
        PortfolioList* currentStock = currentUser->portfolio;
        while (currentStock != NULL) {
            if (currentStock->stock.purchase_price > highestPrice) {
                highestPrice = currentStock->stock.purchase_price;
                strncpy(highestPriceStock, currentStock->stock.symbol, sizeof(highestPriceStock));
            }
            currentStock = currentStock->right;
        }

        currentStock = currentUser->portfolio;
        while (currentStock != NULL) {
            if (currentStock->stock.purchase_price > highestPrice) {
                highestPrice = currentStock->stock.purchase_price;
                strncpy(highestPriceStock, currentStock->stock.symbol, sizeof(highestPriceStock));
            }
            currentStock = currentStock->left;
        }
 
        printf("%-20s Rs. %.2f                %s\n", currentUser->name, portfolioValue, highestPriceStock);
        currentUser = currentUser->next;
    }
}
 
void updateUserPortfolioValue(PortfolioList* root, double transactionValue) {
    if (root == NULL) {
        return;
    }
 
    double currentValue = root->stock.purchase_price * root->stock.quantity;
    double leftValue = calculatePortfolioValue(root->left);
    double rightValue = calculatePortfolioValue(root->right);
 
    double newPortfolioValue = currentValue + leftValue + rightValue + transactionValue;
}
 
 
void displayPortfolio(PortfolioList* root) {
    if (root == NULL) {
        return;
    }
 
    displayPortfolio(root->left);
    printf("Symbol: %s\n", root->stock.symbol);
    printf("Name: %s\n", root->stock.name);
    printf("Purchase Price: Rs. %.2f\n", root->stock.purchase_price);
    printf("Quantity: %d\n", root->stock.quantity);
 
 
 
    printf("\n");
    displayPortfolio(root->right);
}
 
 
double calculateStandardDeviation(PortfolioList* root, char* symbol) {
   
    Stock* stock = find(root, symbol);
 
    printf("\nAttempting to calculate standard deviation for stock %s\n", symbol);
    if (stock != NULL) {
        printf("Historical prices for stock %s: ", stock->symbol);
        for (int i = 0; i < stock->num_prices; ++i) {
            printf("%lf ", stock->historical_prices[i]);
        }
        printf("\n");
    }
 
    if (stock == NULL) {
        printf("\nSymbol not found.\n");
        return -1.0;
    }
 
    if (stock->num_prices < 2) {
        printf("\nInsufficient data points to calculate standard deviation.\n");
        return -1.0;
    }
 
    double sum = 0.0;
    double mean = 0.0;
 
    for (int i = 0; i < stock->num_prices; ++i) {
        sum += stock->historical_prices[i];
    }
    mean = sum / stock->num_prices;
 
    sum = 0.0;
    for (int i = 0; i < stock->num_prices; ++i) {
        sum += pow(stock->historical_prices[i] - mean, 2);
    }
 
    double variance = sum / stock->num_prices;
    return sqrt(variance);
}
 
 
double calculatePortfolioRisk(PortfolioList* root) {
    if (root == NULL) {
        return 0.0;
    }
 
    double currentRisk = calculateStandardDeviation(root, root->stock.symbol);
    printf("\nThe standard deviation for your portfolio is %f\n", currentRisk);
 
    double leftRisk = calculatePortfolioRisk(root->left);
    double rightRisk = calculatePortfolioRisk(root->right);
 
    return currentRisk + leftRisk + rightRisk;
}
 
double calculateNetProfitLoss(Stock stock, int sellQuantity, double sellPrice) {
    double totalSellValue = sellQuantity * sellPrice;
    double totalPurchaseValue = stock.purchase_price * sellQuantity;
 
    double netProfitLoss = totalSellValue - totalPurchaseValue;
 
    if (netProfitLoss >= 0 || sellPrice >= stock.purchase_price) {
 
        return netProfitLoss;
    }
    return netProfitLoss;
}
 
 
 
void sellStock(PortfolioList** root, char* symbol, int sellQuantity, double sellPrice, User* currentUser) {
 
        Stock* stockToSell = find(*root, symbol);
        if (stockToSell) {
            if (stockToSell->quantity >= sellQuantity) {
                double netProfitLoss = calculateNetProfitLoss(*stockToSell, sellQuantity, sellPrice);
                if (netProfitLoss < -currentUser->risk_threshold) {
                    printf("\nTransaction exceeds risk threshold. Sale canceled.\n");
                    return;
                }
                stockToSell->quantity -= sellQuantity;
 
                currentUser->totalNetGainLoss += netProfitLoss;
 
                printf("\nSold %d shares for Rs. %.2f each, totaling Rs. %.2f\n", sellQuantity, sellPrice, sellQuantity * sellPrice);
 
                if (netProfitLoss > 0) {
                    printf("\nYou made a profit of Rs. %.2f per share.\n", netProfitLoss / sellQuantity);
                }
                else if (netProfitLoss < 0) {
                    printf("\nYou incurred a loss of Rs. %.2f per share.\n", -netProfitLoss / sellQuantity);
                }
                else {
                    printf("\nThe sale price exactly matches the purchase price.\n");
                }
 
                if (netProfitLoss > 0) {
                    printf("\nYou made a profit on this transaction.\n");
                    currentUser->portfolio_value += netProfitLoss; 
                }
                else if (netProfitLoss < 0) {
                    printf("\nYou incurred a loss on this transaction.\n");
                    currentUser->portfolio_value -= (-netProfitLoss); 
                }
                else {
                    printf("\nYou broke even on this transaction.\n");
                }
 
            }
            else {
                printf("\nError: Insufficient quantity to sell.\n");
            }
        }
        else {
            printf("\n%s not found in the portfolio.\n", symbol);
        }
    }
 
 
 
PortfolioList* minValueNode(PortfolioList * node) {
        PortfolioList* current = node;
        while (current && current->left != NULL) {
            current = current->left;
        }
        return current;
}
 
PortfolioList* deleteStock(PortfolioList * root, char* symbol) {
        if (root == NULL) {
            return root;
        }
 
        int cmp = strcmp(symbol, root->stock.symbol);
        if (cmp < 0) {
            root->left = deleteStock(root->left, symbol);
        }
        else if (cmp > 0) {
            root->right = deleteStock(root->right, symbol);
        }
        else {
            if (root->left == NULL) {
                PortfolioList* temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL) {
                PortfolioList* temp = root->left;
                free(root);
                return temp;
            }
 
            PortfolioList* temp = minValueNode(root->right);
            strcpy(root->stock.symbol, temp->stock.symbol);
            root->right = deleteStock(root->right, temp->stock.symbol);
        }
        return root;
}
 
 
 
 
void removeStockFromPortfolio(PortfolioList* * root, char* symbol) {
        *root = deleteStock(*root, symbol);
    }
 
void displayROI(User * currentUser) {
        double InitialInvestment;
        double roi;
 
        InitialInvestment = currentUser->initialInvestment;
 
        if (InitialInvestment > 0) {
            roi = ((currentUser->portfolio_value - InitialInvestment) / InitialInvestment) * 100;
            printf("\nUser: %s\n", currentUser->name);
            printf("\nROI: %.2f%%\n", roi);
        }
        else {
            printf("\nNot enough information to calculate ROI.\n");
 
        }
}
 
 
 
void displayStockHistogram(PortfolioList * root) {
        if (root == NULL) {
            return;
        }
 
 
        printf("%s |", root->stock.symbol);
        for (int i = 0; i < root->stock.quantity; i++) {
            printf(" * ");
        }
        printf("\n");
 
 
        displayStockHistogram(root->left);
        displayStockHistogram(root->right);
}
 
 
void updateStockInformation(PortfolioList * root, char* symbol) {
        Stock* stockToUpdate = find(root, symbol);
 
        if (stockToUpdate != NULL) {
 
            printf("\nUpdate Stock Information Menu for %s:\n\n", symbol);
            printf("1. Update Stock Name\n");
            printf("2. Update Purchase Price\n");
            printf("3. Update Quantity\n");
            printf("4. Exit\n");
            printf("\nEnter your choice: ");
            int choice1;
            if (scanf("%d", &choice1) != 1 || choice1 < 1 || choice1 > 4) {
               
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
 
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
 
            }
 
            switch (choice1) {
            case 1: {
                printf("\nEnter new stock name: ");
                scanf("%s", stockToUpdate->name);
                printf("Stock name updated successfully.\n");
                break;
            }
            case 2: {
                printf("\nEnter new purchase price: ");
                scanf("%lf", &stockToUpdate->purchase_price);
                printf("Purchase price updated successfully.\n");
                break;
            }
            case 3: {
                printf("\nEnter new quantity: ");
                scanf("%d", &stockToUpdate->quantity);
                printf("Quantity updated successfully.\n");
                break;
            }
            case 4:
                printf("\nExiting update menu.\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
            }
        }
        else {
            printf("\n%s not found in the portfolio.\n", symbol);
        }
}

static int _GetIntInput(int*    pInput, int isZeroOk)
{
    int input;
    int ret = scanf("%d", &input);
    if(ret == 1) {
        if(input == 0 && isZeroOk == FALSE)
            return 1;

        *pInput = input;
        return 0;
    }

    return 1; /* means error */
}

static int _GetDoubleInput(double*    pInput)
{
    double dInput;
    int ret = scanf("%lf", &dInput);
    if(ret == 1) {
        *pInput = dInput;
        return 0;
    }

    return 1; /* means error */
}

static int _GetStringInput(int nInputChars, char    strInput[])
{
    char sInput[256];
    int ret = scanf("%s", sInput);
    if(ret == 1) {
        strncpy(strInput, sInput, nInputChars);
        strInput[nInputChars - 1] = '\0';
        return 0;
    }

    return 1; /* means error */
}
 
int main() {
    User* users = NULL;

    while (1) {
        printf("\nUser Portfolio Tracker Menu:\n\n");
        printf("1. Create a new user\n");
        printf("2. Enter user's stock portfolio\n");
        printf("3. Display portfolio of all users\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        int choice;
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            printf("\nInvalid choice. Please enter a number between 1 and 4.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1: {
                        char userName[50];
                        printf("\nEnter user name (without spaces please): ");
                        scanf("%s", userName);
                        users = addUser(users, userName);
                        printf("\nUser '%s' added.\n", userName);
                        break;
                    }
            case 2: {
                        char userName[50];
                        printf("\nEnter user name (without spaces please): ");
                        scanf("%s", userName);
                        User* currentUser = findUser(users, userName);
                        if (currentUser == NULL) {
                            printf("\nUser '%s' not found.\n", userName);
                        }
                        else {
                            PortfolioList* userPortfolio = currentUser->portfolio;
                            int portfolioChoice;
                            while (1) {
                                printf("\nUser Portfolio Menu for '%s':\n\n", userName);
                                printf("1. Add a stock to the portfolio\n");
                                printf("2. Find a stock by symbol\n");
                                printf("3. Calculate portfolio value\n");
                                printf("4. Display the portfolio\n");
                                printf("5. Remove a stock from the portfolio\n");
                                printf("6. Update stock information\n");
                                printf("7. Display Stock Portfolio Histogram\n");
                                printf("8. Sell stocks\n");
                                printf("9. ROI(Return on Investment)\n");
                                printf("10.Capital gains tax\n");
                                printf("11. Standard deviation of stocks\n");
                                printf("12. Risk Management\n");
                                printf("13. Exit to main menu\n");
                                printf("\nEnter your choice: ");
                                if (scanf("%d", &portfolioChoice) != 1 || portfolioChoice < 1 || portfolioChoice > 13) {
                                    printf("\nInvalid choice. Please enter a number between 1 and 13.\n");
                                    int c;
                                    while ((c = getchar()) != '\n' && c != EOF);
                                    continue;
                                }
                                switch (portfolioChoice) {
                                    case 1: {
                                                Stock stock;
                                                printf("\nEnter stock symbol: ");
                                                scanf("%s", stock.symbol);
                                                Stock* foundStock = find(userPortfolio, stock.symbol);
                                                if (foundStock != NULL) {
                                                    printf("\nStock with symbol %s is already present in the portfolio of %s. Update stock information if needed using choice 6\n", stock.symbol, userName);
                                                    break;
                                                }
                                                printf("Enter stock name: ");
                                                scanf("%s", stock.name);
                                                printf("Enter purchase price: ");
                                                if(_GetDoubleInput(&stock.purchase_price)) {
                                                    printf("\n Invalid purchase price \n");
                                                    int c;
                                                    while ((c = getchar()) != '\n' && c != EOF);
                                                    break;
                                                }
                                                printf("Enter quantity: ");
                                                if(_GetIntInput(&stock.quantity, FALSE)) {
                                                    printf("\n Invalid quantity \n");
                                                    int c;
                                                    while ((c = getchar()) != '\n' && c != EOF);
                                                    break;
                                                }
                                                printf("\nEnter the number of historical prices for stock %s: ", stock.symbol);
                                                if(_GetIntInput(&stock.num_prices, TRUE)) {
                                                    printf("\n Invalid number of historical prices \n");
                                                    int c;
                                                    while ((c = getchar()) != '\n' && c != EOF);
                                                    break;
                                                }
                                                if(stock.num_prices)
                                                    stock.historical_prices = (double*)malloc(stock.num_prices * sizeof(double));
                                                int allValid = TRUE;
                                                for (int i = 0; i < stock.num_prices; ++i) {
                                                    printf("Enter historical price %d for stock %s: ", i + 1, stock.symbol);
                                                    if(_GetDoubleInput(&stock.historical_prices[i])) {
                                                        printf("\n Invalid historical price \n");
                                                        int c;
                                                        while ((c = getchar()) != '\n' && c != EOF);
                                                        allValid = FALSE;
                                                        break;
                                                    }
                                                }
                                                if(allValid == FALSE) {
                                                    free(stock.historical_prices);
                                                    break;
                                                }
                                                userPortfolio = insert(userPortfolio, stock);
                                                currentUser->portfolio = userPortfolio;
                                                UpdateUserPortfolio( stock, currentUser);
                                                printf("\nStock added to the portfolio.\n");
                                                if(stock.num_prices)
                                                    free(stock.historical_prices);

                                                break;
                                            }
                                    case 2: {
                                                char symbolToFind[10];
                                                printf("\nEnter symbol to find: ");
                                                scanf("%s", symbolToFind);
                                                Stock* foundStock = find(userPortfolio, symbolToFind);
                                                if (foundStock != NULL) {
                                                    printf("\nFound Stock:\n");
                                                    printf("Symbol: %s\n", foundStock->symbol);
                                                    printf("Name: %s\n", foundStock->name);
                                                    printf("Purchase Price: Rs. %.2f\n", foundStock->purchase_price);
                                                    printf("Quantity: %d\n", foundStock->quantity);
                                                }
                                                else {
                                                    printf("\n%s not found in the portfolio.\n", symbolToFind);
                                                }
                                                break;
                                            }
                                    case 3: {
                                                double portfolioValue = calculatePortfolioValue(userPortfolio);
                                                printf("\nPortfolio Value: Rs. %.2f\n", portfolioValue);
                                                break;
                                            }
                                    case 4: {
                                                printf("\nPortfolio for user '%s':\n", userName);
                                                displayPortfolio(userPortfolio);
                                                break;
                                            }
                                    case 5: {
                                                char symbolToRemove[10];
                                                printf("\nEnter symbol to remove: ");
                                                scanf("%s", symbolToRemove);
                                                userPortfolio = deleteStock(userPortfolio, symbolToRemove);
                                                currentUser->portfolio = userPortfolio;
                                                printf("\nStock removed from the portfolio.\n");
                                                break;
                                            }
                                    case 6: {
                                                char symbolToUpdate[10];
                                                printf("\nEnter symbol to update: ");
                                                scanf("%s", symbolToUpdate);
                                                updateStockInformation(userPortfolio, symbolToUpdate);
                                                break;
                                            }

                                    case 7: {
                                                printf("\nStock Quantity Histogram:\n");
                                                displayStockHistogram(userPortfolio);
                                                break;
                                            }

                                    case 8: {
                                                char symbolToSell[10];
                                                int sellQuantity;
                                                double sellPrice;

                                                printf("\nEnter symbol to sell: ");
                                                scanf("%s", symbolToSell);
                                                printf("Enter quantity to sell: ");
                                                if(_GetIntInput(&sellQuantity, FALSE)) {
                                                    printf("\n Invalid quantity \n");
                                                    int c;
                                                    while ((c = getchar()) != '\n' && c != EOF);
                                                    break;
                                                }
                                                printf("Enter sell price: ");
                                                if(_GetDoubleInput(&sellPrice)) {
                                                    printf("\n Invalid sell price \n");
                                                    int c;
                                                    while ((c = getchar()) != '\n' && c != EOF);
                                                    break;
                                                }

                                                sellStock(&(currentUser->portfolio), symbolToSell, sellQuantity, sellPrice, currentUser);
                                                break;
                                            }
                                    case 9: {
                                                displayROI(currentUser);
                                                break;
                                            }
                                    case 10: {
                                                double capitalGainsTaxRate = 0.15;
                                                double capitalGainsTax = currentUser->totalNetGainLoss * capitalGainsTaxRate;
                                                printf("\nCapital Gains Tax: Rs. %.2f\n", capitalGainsTax);
                                                break;
                                            }
                                    case 11: {
                                                 char symbolToCalculateSD[10];
                                                 printf("\nEnter symbol to calculate standard deviation: ");
                                                 scanf("%s", symbolToCalculateSD);
                                                 double sd = calculateStandardDeviation(currentUser->portfolio, symbolToCalculateSD);
                                                 printf("\nStandard Deviation for %s: %.2f\n", symbolToCalculateSD, sd);
                                                 printf("Look into risk management(option 12) to figure out your next steps\n");
                                                 break;
                                             }
                                    case 12: {
                                                 double currentRisk;
                                                 currentRisk = calculatePortfolioRisk(currentUser->portfolio);
                                                 printf("\nAfter analysing your standard deviation, Current Risk Exposure for %s: %.2f\n", currentUser->name, currentRisk);
                                                 if (currentRisk > currentUser->risk_threshold) {
                                                     printf("\nRisk exposure exceeds the threshold. Please look into managing your risk\n");
                                                     printf("Consider diversifying or rebalancing your portfolio\n");
                                                     break;
                                                 }
                                             }
                                    case 13: {
                                                 break;
                                             }
                                    default:
                                             printf("\nInvalid choice. Please try again.\n");
                                }

                                if (portfolioChoice == 13) {
                                    break;
                                }
                            }
                        }
                        break;
                    }
            case 3: {
                        displayAllUsersPortfolio(users);
                        break;
                    }
            case 4: {
                        return 0;
                    }
            default:
                    printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}