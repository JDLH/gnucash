/********************************************************************\
 * Group.h -- the main data structure of the program                *
 * Copyright (C) 1997 Robin D. Clark                                *
 * Copyright (C) 1997, 1998 Linas Vepstas                           *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, write to the Free Software      *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.        *
 *                                                                  *
 *   Author: Rob Clark                                              *
 * Internet: rclark@cs.hmc.edu                                      *
 *  Address: 609 8th Street                                         *
 *           Huntington Beach, CA 92648-4632                        *
\********************************************************************/

#ifndef __XACC_ACCOUNT_GROUP_H__
#define __XACC_ACCOUNT_GROUP_H__

#include "config.h"
#include "gnc-common.h"

#include "Account.h"

/** PROTOTYPES ******************************************************/
AccountGroup    *xaccMallocAccountGroup( void );
void    xaccFreeAccountGroup( AccountGroup *account_group );

/*
 * The xaccConcatGroups() subroutine will move all accounts
 *    from the "from" group to the "to" group
 *
 * The xaccMergeAccounts() subroutine will go through a group,
 *    merging all accounts that have the same name and description.
 *    This function is useful when importing Quicken(TM) files.
 */
void    xaccConcatGroups (AccountGroup *to, AccountGroup *from);
void    xaccMergeAccounts (AccountGroup *grp);

/*
 * The xaccAccountGroupNotSaved() subroutine will return 
 *    a non-zero value if any account in the group or in
 *    any subgroup hasn't been saved.
 *
 * The xaccAccountGroupMarkSaved() subroutine will mark
 *    the entire group as having been saved, including 
 *    all of the child accounts.
 */
int     xaccAccountGroupNotSaved  (AccountGroup *grp);
void    xaccAccountGroupMarkSaved (AccountGroup *grp);

/*
 * The xaccRemoveAccount() subroutine will remove the indicated
 *    account from its parent account group. It will NOT free the
 *    associated memory or otherwise alter the account: the account
 *    can now be reparented to a new location.
 *    Note, however, that it will mark the old parents as having 
 *    been modified.
 *
 * The xaccRemoveGroup() subroutine will remove the indicated
 *    account group from its parent account. It will NOT free the
 *    associated memory or otherwise alter the account group: the 
 *    account group can now be reparented to a new location.
 *    Note, however, that it will mark the old parents as having 
 *    been modified.
 */
void    xaccRemoveAccount (Account *);
void    xaccRemoveGroup (AccountGroup *);
void    xaccGroupInsertAccount( AccountGroup *grp, Account *acc );
void    xaccInsertSubAccount( Account *parent, Account *child );

/*
 * The xaccGetNumAccounts() subroutine returns the number
 *    of accounts, including subaccounts, in the account group
 *
 * The xaccGroupGetNumAccounts() subroutine returns the number
 *    of accounts in the indicated group only (children not counted).
 *
 * The xaccGroupGetDepth() subroutine returns the length of the 
 *    longest tree branch.  Each link between an account and its
 *    (non-null) children counts as one unit of length.
 */
int     xaccGetNumAccounts (AccountGroup *grp);
int     xaccGroupGetNumAccounts (AccountGroup *grp);
int     xaccGroupGetDepth (AccountGroup *grp);

/*
 * The xaccGetAccounts() subroutine returns an array containing 
 *    all of the accounts, including subaccounts, in the account group.
 *    The returned array should be freed when no longer needed.
 *
 * The xaccFillInAccounts() routine performs the same function as the
 *    above routine, except that it fills in the array provided by the
 *    user.  The array provioded by the user *must* be large enough,
 *    including a terminating NULL pointer.
 */
Account ** xaccGetAccounts (AccountGroup *grp);
int        xaccFillInAccounts ( AccountGroup *root, Account **arr );

/* 
 * The xaccGetAccountFromID() subroutine fetches the account
 *    with the indicated account id from the collection of accounts
 *    in the indicated AccountGroup.  It returns NULL if the 
 *    account was not found.
 *
 * The xaccGetPeerAccountFromID() subroutine fetches the account
 *    with the indicated account id from the collection of accounts
 *    in the same AccountGroup anchor group. It returns NULL if the
 *    account was not found.
 *
 * The xaccGetAccountFromName() subroutine fetches the
 *    account by name from the collection of accounts
 *    in the indicated AccountGroup group.  It returns NULL if the
 *    account was not found.
 *
 * The xaccGetPeerAccountFromName() subroutine fetches the
 *    account by name from the collection of accounts
 *    in the same AccountGroup anchor group. It returns NULL if the
 *    account was not found.
 */

Account *xaccGetAccountFromID       (AccountGroup *, int);
Account *xaccGetPeerAccountFromID   (Account *, int);
Account *xaccGetAccountFromName     (AccountGroup *, const char *);
Account *xaccGetPeerAccountFromName (Account *, const char *);

/*
 * The xaccRecomputeGroupBalance() subroutine recursively totals
 *    up the balances of all accounts in a group.
 */

void xaccRecomputeGroupBalance (AccountGroup *);

/*
 * The xaccGroupGetBalance() method returns the total of the balances 
 *    of all the children in this group.
 */

double    xaccGroupGetBalance (AccountGroup *);

/*
 * The xaccGetAccountRoot () subroutine will find the topmost 
 *    (root) group to which this account belongs.
 */

AccountGroup * xaccGetAccountRoot (Account *);

/* The xaccConsolidateGrpTrans() subroutine scans through
 *    all of the transactions in an account, and compares them.
 *    if any of them are exact duplicates, the duplicates are removed.
 *    duplicates may occur when accounts from multiple sources are 
 *    merged.  Note that this can be a dangerous operation to perform 
 *
 *    Note that this suborutine merely walks the acount group
 *    tree, and calls ConsolidateTransacations on each account
 */

void xaccConsolidateGrpTransactions (AccountGroup *);

Account * xaccGroupGetAccount (AccountGroup *, int);

/*
 * The xaccGroupGetNextFreeCode() method will try to guess a reasonable 
 *    candidate for the next unused account code in this group.
 *
 * The xaccAccountGetNextChildCode() method does same as above,
 *    except that it returns a value appropriate for a child account.
 *
 * The xaccGroupAutoCode() method will traverse the group, automatically
 *    inserting account codes into those accounts whose account codes 
 *    are blank.  It uses the algorithm used in xaccAccountAutoCode()
 *    to pick an account code.
 *
 * The xaccGroupDepthAutoCode() first measures teh depth of the account
 *    tree, and uses that depth to pck the number of digits in the account
 *    code.
 */

char * xaccGroupGetNextFreeCode (AccountGroup *grp, int num_digits);
char * xaccAccountGetNextChildCode (Account *acc, int num_digits);
void   xaccGroupAutoCode (AccountGroup *grp, int num_digits);
void   xaccGroupDepthAutoCode (AccountGroup *grp);

#ifndef SWIG

/*
 * The following functions provide support for "staged traversals"
 * over all of the transactions in and account or group.  The idea
 * is to be able to perform a sequence of traversals ("stages"),
 * and perform an operation on each transaction exactly once 
 * for that stage.  
 * Only transactions whose current "stage" is less than the
 * stage of the current traversal will be affected, and they will be
 * "brought up" to the current stage when they are processed.
 *
 * For example, you could perform a stage 1 traversal of all the
 * transactions in an account, and then perform a stage 1 traversal of
 * the transactions in a second account.  Presuming the traversal of
 * the first account didn't abort prematurely, any transactions shared
 * by both accounts would be ignored during the traversal of the
 * second account since they had been processed while traversing the
 * first account.
 *
 * However, if you had traversed the second account using a stage 
 * of 2, then all the transactions in the second account would have 
 * been processed.
 *
 * Traversal can be aborted by having the callback function return a
 * non-zero value.  The traversal is aborted immediately, and the 
 * non-zero value is returned.  Note that an aborted traversal can 
 * be restarted; no information is lost due to an abort.
 *
 * The initial impetus for this particular approach came from
 * generalizing a mark/sweep practice that was already being used in
 * FileIO.c.
 *
 * Note that currently, there is a hard limit of 256 stages, which
 * can be changed by enlarging "marker" in the tranaction struct.
 * */

/* xaccGroupBeginStagedTransactionTraversals() resets the traversal
 *    marker inside each of all the transactions in the group so that a
 *    new sequence of staged traversals can begin. 
 */

void xaccGroupBeginStagedTransactionTraversals(AccountGroup *grp);

/* xaccGroupStagedTransactionTraversal() calls thunk on each
 *    transaction in the group whose current marker is less than the
 *    given `stage' and updates each transaction's marker to be `stage'.
 *    The traversal will stop if thunk() returns a non-zero value.
 *    xaccGroupStagedTransactionTraversal() function will return zero 
 *    or the non-zero value returned by thunk().  This
 *    API does not handle handle recursive traversals.
 *
 *    Currently the result of adding or removing transactions during a
 *    traversal is undefined, so don't do that.
 */

int 
xaccGroupStagedTransactionTraversal(AccountGroup *grp,
                                    unsigned int stage,
                                    int (*thunk)(Transaction *t, void *data),
                                    void *data);

/* xaccAccountStagedTransactionTraversal() calls thunk on each
 *    transaction in the account whose current marker is less than the
 *    given `stage' and updates each transaction's marker to be `stage'.
 *    The traversal will stop if thunk() returns a non-zero value.
 *    xaccAccountStagedTransactionTraversal() function will return zero
 *    or the non-zero value returned by thunk().
 *    This API does not handle handle recursive traversals.
 *
 *    Currently the result of adding or removing transactions during a
 *    traversal is undefined, so don't do that. 
 */

int xaccAccountStagedTransactionTraversal(Account *a,
                                          unsigned int stage,
                                          int (*thunk)(Transaction *t, void *data),
                                          void *data);

#endif

#endif /* __XACC_ACCOUNT_GROUP_H__ */