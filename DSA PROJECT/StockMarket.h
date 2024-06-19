#ifndef StockMarket_h
#define StockMarket_h

typedef struct Stock {
    char symbol[10];
    char name[50];
    double purchase_price;
    int quantity;
    double* historical_prices;  
    int num_prices;
} Stock;

typedef struct PortfolioList {
    Stock stock;
    struct PortfolioList* left;
    struct PortfolioList* right;
} PortfolioList;

typedef struct User {
    char name[50];
    PortfolioList* portfolio;
    struct User* next;
    double portfolio_value;
    double totalNetGainLoss;
    char highest_priced_stock[10];
    double risk_threshold;
    double initialInvestment;
} User;

#endif /* #ifndef StockMarket_h */