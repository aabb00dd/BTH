# Website Development with Git

## Getting Started

## To work on this project locally, follow these steps:

### 1. Clone the Repository

```bash
git clone <repository_url>
cd <repository_name>
```

Replace <repository_url> with the actual [GitHub URL of the repository](https://github.com/Gingertorin/hemsida_bth_halsa.git)
Replace <repository_name> with the actual local directory (eg hemsida_bth_halsa/)

### 2. Check Out Your Branch

If your branch already exists locally, switch to it:

```bash
git checkout <branch_name (eg 'elias')>
```

If the branch does not exist locally, fetch it from the remote repository and check it out:

```bash
git fetch origin
git checkout -b <branch_name (eg 'elias')>
```

### 3. Pull Latest Changes

Ensure your local branch is up-to-date before making changes:

```bash
git pull origin <branch_name (eg 'elias')>
```

### 4. Check Available Branches

To list all local branches:

```bash
git branch
```

To list all remote branches:

```bash
git branch -r
```

To list both local and remote branches:

```bash
git branch -a
```

### 5. Make and Track Changes

After modifying files, check which changes have been made:

```bash
git status
```

To stage all changed files for commit:

```bash
git add .
```

To only stage a specific part of your changes, use:

```bash
git add "directory or file name"
```

### 6. Commit Changes

Commit your changes with a meaningful message:

```bash
git commit -m "Describe your changes here"
```

### 7. Push Changes to Your Branch

Push the committed changes to your remote branch (eg 'elias'):

```bash
git push origin <branch_name (eg elias)>
```

### 8. Create a Pull Request

After pushing to your branch, go to GitHub and create a Pull Request (PR) to merge your branch into main. Wait for merge approval by another team member.

### 9. Merging the Latest Changes from `main` into Your Branch

To keep your branch up to date with the latest changes from `main`, follow these steps:

```bash
git fetch origin
```

Make sure you are in your branch:

```bash
git branch -a
```

Merge the latest `main` changes into your branch:

```bash
git merge origin/main
```

Push the updated branch back to GitHub:

```bash
git push origin <branch_name>
```

This ensures your branch stays in sync with the latest project updates before submitting a pull request.

### 10. Merging the Latest Changes from `other_persons_branch` into your Branch

To keep your branch up to date with the latest changes from `other_persons_branch`, follow these steps:

```bash
git fetch origin
```

Make sure you are in your branch:

```bash
git branch -a
```

Merge the latest `other_persons_branch` changes into your branch:

```bash
git merge origin/<other_persons_branch>
```

To update your remote branch, push the updated branch back to GitHub:

```bash
git push origin <your_branch_name>
```

### 11. How to make sure that your branch is exactly as main - removing any local changes.

Update your local main branch
```bash
git checkout main
git pull origin main
```

Switch to your local branch
```bash
git checkout elias
```

Reset your branch to match main
```bash
git reset --hard main
```

Remove untracked files and directories
```bash
git clean -fd
```
