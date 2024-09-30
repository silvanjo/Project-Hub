import csv
import os
from datetime import datetime

# Load transactions from the CSV file
def load_transactions(filename="finance.csv"):
    transactions = []
    if os.path.exists(filename):
        with open(filename, mode="r") as file:
            reader = csv.DictReader(file)
            for row in reader:
                transactions.append(row)
    return transactions

# Save transactions to the CSV file
def save_transactions(transactions, filename="finance.csv"):
    with open(filename, mode="w", newline="") as file:
        fieldnames = ["date", "category", "type", "amount", "description"]
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(transactions)

# Add a new transaction
def add_transaction(transactions):
    date = input("Enter date (YYYY-MM-DD): ")
    category = input("Enter category (e.g., groceries, salary, utilities): ")
    trans_type = input("Enter transaction type (income/expense): ").lower()
    amount = input("Enter amount: ")
    description = input("Enter description: ")

    transactions.append({
        "date": date,
        "category": category,
        "type": trans_type,
        "amount": amount,
        "description": description
    })
    print("Transaction added successfully!")

# View all transactions
def view_transactions(transactions):
    if not transactions:
        print("No transactions found.")
        return

    print(f"{'Date':<12} {'Category':<12} {'Type':<10} {'Amount':<10} {'Description'}")
    for transaction in transactions:
        print(f"{transaction['date']:<12} {transaction['category']:<12} {transaction['type']:<10} "
              f"{transaction['amount']:<10} {transaction['description']}")

# Filter transactions by date range, category, or type
def filter_transactions(transactions):
    filter_type = input("Filter by (date/category/type): ").lower()

    if filter_type == "date":
        start_date = input("Enter start date (YYYY-MM-DD): ")
        end_date = input("Enter end date (YYYY-MM-DD): ")
        filtered = [t for t in transactions if start_date <= t["date"] <= end_date]
    elif filter_type == "category":
        category = input("Enter category: ").lower()
        filtered = [t for t in transactions if t["category"].lower() == category]
    elif filter_type == "type":
        trans_type = input("Enter transaction type (income/expense): ").lower()
        filtered = [t for t in transactions if t["type"] == trans_type]
    else:
        print("Invalid filter type.")
        return

    if filtered:
        print(f"{'Date':<12} {'Category':<12} {'Type':<10} {'Amount':<10} {'Description'}")
        for transaction in filtered:
            print(f"{transaction['date']:<12} {transaction['category']:<12} {transaction['type']:<10} "
                  f"{transaction['amount']:<10} {transaction['description']}")
    else:
        print(f"No transactions found for the given {filter_type}.")

# Update an existing transaction
def update_transaction(transactions):
    description = input("Enter the description of the transaction to update: ")
    
    for transaction in transactions:
        if transaction["description"] == description:
            print(f"Current Record: {transaction}")
            transaction["date"] = input(f"Enter new date (YYYY-MM-DD) or press Enter to keep {transaction['date']}: ") or transaction["date"]
            transaction["category"] = input(f"Enter new category or press Enter to keep {transaction['category']}: ") or transaction["category"]
            transaction["type"] = input(f"Enter new type (income/expense) or press Enter to keep {transaction['type']}: ") or transaction["type"]
            transaction["amount"] = input(f"Enter new amount or press Enter to keep {transaction['amount']}: ") or transaction["amount"]
            transaction["description"] = input(f"Enter new description or press Enter to keep {transaction['description']}: ") or transaction["description"]
            print("Transaction updated successfully!")
            return
    print("Transaction not found.")

# Delete a transaction
def delete_transaction(transactions):
    description = input("Enter the description of the transaction to delete: ")

    for transaction in transactions:
        if transaction["description"] == description:
            transactions.remove(transaction)
            print("Transaction deleted successfully!")
            return
    print("Transaction not found.")

# Generate a summary report
def generate_summary(transactions):
    total_income = sum(float(t["amount"]) for t in transactions if t["type"] == "income")
    total_expense = sum(float(t["amount"]) for t in transactions if t["type"] == "expense")
    balance = total_income - total_expense

    print(f"\nSummary Report")
    print(f"Total Income: {total_income}")
    print(f"Total Expenses: {total_expense}")
    print(f"Net Balance: {balance}")

    # Optional: Generate report by category
    category_totals = {}
    for transaction in transactions:
        category = transaction["category"]
        amount = float(transaction["amount"])
        if transaction["type"] == "expense":
            category_totals[category] = category_totals.get(category, 0) + amount

    print("\nExpenses by Category:")
    for category, total in category_totals.items():
        print(f"{category}: {total}")

# Main menu function
def main():
    transactions = load_transactions()

    while True:
        print("\nPersonal Finance Tracker")
        print("1. Add Transaction")
        print("2. View All Transactions")
        print("3. Filter Transactions")
        print("4. Update Transaction")
        print("5. Delete Transaction")
        print("6. Generate Summary Report")
        print("7. Exit")

        choice = input("Enter your choice: ")

        if choice == "1":
            add_transaction(transactions)
        elif choice == "2":
            view_transactions(transactions)
        elif choice == "3":
            filter_transactions(transactions)
        elif choice == "4":
            update_transaction(transactions)
        elif choice == "5":
            delete_transaction(transactions)
        elif choice == "6":
            generate_summary(transactions)
        elif choice == "7":
            save_transactions(transactions)
            print("Transactions saved. Exiting...")
            break
        else:
            print("Invalid choice, please try again.")

if __name__ == "__main__":
    main()

