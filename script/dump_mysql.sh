#!/bin/bash

MAXIMUM_BACKUP_FILES=0              #最大备份文件数，为0表示保存所有已有备份
BACKUP_FOLDERNAME="mysql_backup"
DB_HOSTNAME="localhost"
DB_USERNAME="boostme"
DB_PASSWORD="boostme"
DATABASES=(
"boostme"
)

#CURRENT_DATE=$(date +%F)
CURRENT_DATE=$(date +%Y%m%d-%H%M%S)
BACKUP_FOLDER="${BACKUP_FOLDERNAME}_${CURRENT_DATE}"

mkdir -p $BACKUP_FOLDER
cnt=0
while [ -n "${DATABASES[cnt]}" ]; do
    cnt=$((cnt + 1))
done

echo "[+] ${cnt} databases will be backuped..."
for DATABASE in ${DATABASES[@]}; do
    printf "[+] Begining Dumping Database [${DATABASE}] Date [%s]\n" "$(date)"
    if $(mysqldump -h ${DB_HOSTNAME} -u${DB_USERNAME} -p${DB_PASSWORD} ${DATABASE} > "${BACKUP_FOLDER}/${DATABASE}.sql"); then
        echo "Dumped [${DATABASE}] successfully!"
    else
        echo "Failed dumping database [${DATABASE}]!"
    fi
done

echo "[+] Packaging and compressing the backup folder..."
tar -cv ${BACKUP_FOLDER} | bzip2 > ${BACKUP_FOLDER}.tar.bz2 && rm -rf $BACKUP_FOLDER
BACKUP_FILES_MADE=$(($(ls -l ${BACKUP_FOLDERNAME}*.tar.bz2 | wc -l) - 0))

echo "[+] There are ${BACKUP_FILES_MADE} backup files actually."
if [ $MAXIMUM_BACKUP_FILES -ne 0 -a $BACKUP_FILES_MADE -gt $MAXIMUM_BACKUP_FILES ];then
    REMOVE_FILES=$(($BACKUP_FILES_MADE - $MAXIMUM_BACKUP_FILES))
    echo "[+] Remove ${REMOVE_FILES} old backup files."

    ALL_BACKUP_FILES=($(ls -t ${BACKUP_FOLDERNAME}*.tar.bz2))
    SAFE_BACKUP_FILES=("${ALL_BACKUP_FILES[@]:0:${MAXIMUM_BACKUP_FILES}}")
    echo "[+] Safeting the newest backup files and removing old files..."
    FOLDER_SAFETY="_safety"
    if [ ! -d $FOLDER_SAFETY ]
        then mkdir $FOLDER_SAFETY
    fi
    for FILE in ${SAFE_BACKUP_FILES[@]};do
        mv -i  ${FILE}  ${FOLDER_SAFETY}
    done
    rm -rf ${BACKUP_FOLDERNAME}*.tar.bz2
    mv  -i ${FOLDER_SAFETY}/* ./
    rm -rf ${FOLDER_SAFETY}
fi
