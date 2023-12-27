#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure pour représenter une transaction
struct transaction {
    char type; // 'W' pour retrait, 'D' pour dépôt
    double amount;
    char date[20]; // Vous devrez peut-être utiliser un type de données approprié pour la date
};

// Structure pour représenter un utilisateur
struct user {
    char *fName;
    char *lName;
    char *CIN;
    double sum;
    struct transaction *transactionHistory;
    int transactionCount; // Pour suivre le nombre de transactions
};

// Fonction pour ajouter une transaction à l'historique
void addTransaction(struct user *account, char type, double amount) {
    // Réallocation de mémoire pour la nouvelle transaction
    account->transactionCount++;
    account->transactionHistory = realloc(account->transactionHistory, account->transactionCount * sizeof(struct transaction));

    // Ajout de la nouvelle transaction à la fin de l'historique
    struct transaction *newTransaction = &(account->transactionHistory[account->transactionCount - 1]);
    newTransaction->type = type;
    newTransaction->amount = amount;

}

// Fonction pour afficher l'historique des transactions d'un compte
void displayTransactionHistory(struct user *account) {
    printf("\nHistorique des transactions pour le compte %s : ", account->CIN);
    printf("\nType\t|\tMontant\t|\tDate");
    for (int i = 0; i < account->transactionCount; i++) {
        printf("\n%c\t|\t%lf\t|\t%s", account->transactionHistory[i].type,
               account->transactionHistory[i].amount, account->transactionHistory[i].date);
    }
}

// Fonction pour effectuer un retrait sur un compte
void withdraw(struct user *userAccounts, int currentCount) {
    char searchFor[50];
    double amountToWithdraw;
    struct user *accountToWithdrawFrom;

    printf("\nEntrer le CIN du compte : ");
    scanf(" %s", searchFor);

    accountToWithdrawFrom = search(searchFor, userAccounts, currentCount);

    if (accountToWithdrawFrom == NULL) {
        printf("\nCompte non trouvé.");
        return;
    }

    printf("\nMontant actuel : %lf", accountToWithdrawFrom->sum);
    printf("\nEntrer le montant à retirer : ");
    scanf(" %lf", &amountToWithdraw);

    if (amountToWithdraw > accountToWithdrawFrom->sum) {
        printf("\nLe montant à retirer dépasse le solde du compte.");
        return;
    }

    accountToWithdrawFrom->sum -= amountToWithdraw;
    printf("\nNouveau solde : %lf", accountToWithdrawFrom->sum);

    // Ajouter la transaction à l'historique
    addTransaction(accountToWithdrawFrom, 'W', amountToWithdraw);
}
// Fonction pour effectuer un dépôt sur un compte
void deposit(struct user *userAccounts, int currentCount) {
    char searchFor[50];
    double amountToDeposit;
    struct user *accountToDepositIn;

    printf("\nEntrer le CIN du compte : ");
    scanf(" %s", searchFor);

    accountToDepositIn = search(searchFor, userAccounts, currentCount);

    if (accountToDepositIn == NULL) {
        printf("\nCompte non trouvé.");
        return;
    }

    printf("\nMontant actuel : %lf", accountToDepositIn->sum);
    printf("\nEntrer le montant à déposer : ");
    scanf(" %lf", &amountToDeposit);

    accountToDepositIn->sum += amountToDeposit;
    printf("\nNouveau solde : %lf", accountToDepositIn->sum);

    // Ajouter la transaction à l'historique
    addTransaction(accountToDepositIn, 'D', amountToDeposit);
}
// Fonction pour la fidélisation des clients
void loyalty(struct user *userAccounts, int currentCount) {
    char choice;
    int numAccounts = 3;

    if (currentCount <= 3) {
        numAccounts = currentCount;
    }

    // Trier les comptes par ordre décroissant
    selectionSortDouble(false, 'd', 0.0, userAccounts, currentCount);

    printf("\nLes 3 comptes ayant le montant le plus élevé : ");
    printf("\nNom\t|\tPrenom\t|\tCIN\t|\tMontant");

    for (int i = 0; i < numAccounts; i++) {
        printf("\n%s\t|\t%s\t|\t%s\t|\t%lf", userAccounts[i].lName, userAccounts[i].fName,
               userAccounts[i].CIN, userAccounts[i].sum);
    }

    printf("\nVoulez-vous ajouter 1.3 pourcent à ces comptes? Y/N: ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        for (int i = 0; i < numAccounts; i++) {
            userAccounts[i].sum += userAccounts[i].sum * (1.3 / 100);
        }

        printf("\nMontant mis à jour après fidélisation : ");
        printf("\nNom\t|\tPrenom\t|\tCIN\t|\tMontant");

        for (int i = 0; i < numAccounts; i++) {
            printf("\n%s\t|\t%s\t|\t%s\t|\t%lf", userAccounts[i].lName, userAccounts[i].fName,
                   userAccounts[i].CIN, userAccounts[i].sum);
        }
    }
}

int main() {
    char choice = '6';
    int i = 0, j;
    int accountsNum;
    bool repeat = true;
    int initialStrucSize = 1;
    struct user *userAccounts = NULL; // Initialisez à NULL pour la première allocation

    do {
        accountsNum = 1;
        if (userAccounts != NULL) menuDisplay('p', &choice);

        switch (choice) {
            // ... (Les autres cas du menu principal)

            case '4': // affichage
                if (i == 0) {
                    printf("\nLa base de donnees est vide. Essayer d'introduire des nouveaux comptes.");
                    break;
                }
                menuDisplay('a', &choice);
                if (choice == '7') { repeat = false; break; }
                if (choice == '6') break;
                affichage(choice, userAccounts, i);

                // Ajoutez un cas pour afficher l'historique des transactions
                if (choice == '8') {
                    printf("\nEntrer le CIN du compte pour afficher l'historique des transactions : ");
                    char searchFor[50];
                    scanf(" %s", searchFor);
                    struct user *accountToSearch = search(searchFor, userAccounts, i);
                    if (accountToSearch == NULL) { printf("\nCIN introuvable"); break; }
                    displayTransactionHistory(accountToSearch);
                }

                break;

// Fonction pour créer un nouveau compte
void createAccounts(int *currentCount, int accountsNum, struct user *pStruct) {
    int i = (*currentCount);
    int maxCount = (*currentCount) + accountsNum;

    for (i; i < maxCount; i++) {
        printf("\nEntrer le CIN de l'utilisateur : ");
        scanf(" %s", pStruct[i].CIN);

        // Rechercher si le CIN existe déjà
        if (search(pStruct[i].CIN, pStruct, *currentCount) != NULL) {
            printf("\nCIN déjà existant, veuillez essayer avec un autre.");
            i--;
            continue;
        }

        printf("\nEntrer le nom de l'utilisateur du compte : ");
        scanf(" %s", pStruct[i].lName);

        printf("\nEntrer le prénom de l'utilisateur : ");
        scanf(" %s", pStruct[i].fName);

        printf("\nEntrer le montant : ");
        scanf(" %lf", &(pStruct[i].sum));

        (*currentCount) += 1;
    }
}
// Fonction pour rechercher un compte par CIN
struct user *search(char *searchFor, struct user *userAccounts, int currentCount) {
    int i;

    for (i = 0; i < currentCount; i++) {
        if (!strcmp(searchFor, userAccounts[i].CIN)) {
            return &userAccounts[i];
        }
    }

    printf("\nCe CIN ne figure pas dans la base de données.");
    return NULL;
}
// Fonction pour afficher les détails des comptes selon le choix de l'utilisateur
void affichage(char choice, struct user *userAccounts, int currentCount) {
    char subChoice;
    char searchFor[50];
    struct user *accountToSearch;
    double minSumToDisplay = 0.0;

    switch (choice) {
        case '1': // Trier en ascendant
            printf("\nTrier en ascendant par :\n1. Montant \n2. Nom \n3. Prenom\n");
            scanf(" %c", &subChoice);
            sortFunction(&subChoice, 'a', minSumToDisplay, userAccounts, currentCount);
            break;
        case '2': // Trier en descendant
            printf("\nTrier en descendant par :\n1. Montant \n2. Nom \n3. Prenom\n");
            scanf(" %c", &subChoice);
            sortFunction(&subChoice, 'd', minSumToDisplay, userAccounts, currentCount);
            break;
        case '3':  // Montant > ? Ordre Ascendant
            printf("\nEntrer le montant minimum à afficher : ");
            scanf(" %lf", &minSumToDisplay);
            subChoice = '1';
            sortFunction(&subChoice, 'a', minSumToDisplay, userAccounts, currentCount);
            break;
        case '4': // Montant > ? Ordre Descendant
            printf("\nEntrer le montant minimum à afficher : ");
            scanf(" %lf", &minSumToDisplay);
            subChoice = '1';
            sortFunction(&subChoice, 'd', minSumToDisplay, userAccounts, currentCount);
            break;
        case '5': // Recherche par CIN
            printf("\nEntrer le CIN à rechercher : ");
            scanf(" %s", searchFor);
            accountToSearch = search(searchFor, userAccounts, currentCount);

            if (accountToSearch == NULL) {
                printf("\nCIN introuvable");
                break;
            }

            printf("\nNom\t|\tPrenom\t|\tCIN\t|\tMontant");
            printf("\n%s\t|\t%s\t|\t%s\t|\t%lf", accountToSearch->lName, accountToSearch->fName,
                   accountToSearch->CIN, accountToSearch->sum);
            break;
        case '6': // Retourner au menu principal
            system("cls");
            break;
    }
}

        } while (repeat);

        // Libérez la mémoire allouée
        free(userAccounts);
        for (j = 0; j < i + 1; j++) {
            free((userAccounts + j)->fName);
            free((userAccounts + j)->lName);
            free((userAccounts + j)->CIN);
            free((userAccounts + j)->transactionHistory);
        }

        return 0;
    }
