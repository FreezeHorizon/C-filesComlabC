#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

#define MAX_PATH_LEN 1024

char from_ext[16];
char to_ext[16];
char output_base[MAX_PATH_LEN];

// Helper to check if file ends with a given extension
int has_extension(const char *filename, const char *ext) {
    const char *dot = strrchr(filename, '.');
    return dot && strcmp(dot, ext) == 0;
}

// Replace extension in filename
void replace_extension(const char *filename, char *new_name, const char *new_ext) {
    const char *dot = strrchr(filename, '.');
    if (!dot) {
        snprintf(new_name, MAX_PATH_LEN, "%s%s", filename, new_ext);
    } else {
        int diff = (int)(dot - filename);
        size_t base_len = diff >= 0 ? (size_t)diff : 0;
        if (diff < 0) base_len = 0;
        strncpy(new_name, filename, base_len);
        new_name[base_len] = '\0';
        strcat(new_name, new_ext);
    }
}

// Join paths
void join_paths(const char *base, const char *name, char *result) {
    snprintf(result, MAX_PATH_LEN, "%s/%s", base, name);
}

// Copy file with new extension
void copy_and_rename_file(const char *src_path, const char *dst_path) {
    FILE *src = fopen(src_path, "rb");
    if (!src) {
        perror("Failed to open source file");
        return;
    }

    FILE *dst = fopen(dst_path, "wb");
    if (!dst) {
        perror("Failed to open destination file");
        fclose(src);
        return;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }

    fclose(src);
    fclose(dst);
    printf("Converted: %s -> %s\n", src_path, dst_path);
}

// Recursively process directory
void process_directory(const char *input_dir, const char *output_dir) {
    DIR *dir = opendir(input_dir);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char input_path[MAX_PATH_LEN], output_path[MAX_PATH_LEN];
        join_paths(input_dir, entry->d_name, input_path);
        join_paths(output_dir, entry->d_name, output_path);

        struct stat st;
        if (stat(input_path, &st) == -1) {
            perror("stat failed");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            char answer[4];
            printf("Enter directory: %s — convert files inside? (yes/no): ", input_path);
            scanf("%3s", answer);
            if (strcmp(answer, "yes") == 0) {
                MKDIR(output_path);
                process_directory(input_path, output_path);
            }
        } else if (S_ISREG(st.st_mode) && has_extension(entry->d_name, from_ext)) {
            char new_name[MAX_PATH_LEN];
            replace_extension(entry->d_name, new_name, to_ext);
            join_paths(output_dir, new_name, output_path);
            copy_and_rename_file(input_path, output_path);
        }
    }

    closedir(dir);
}

int main() {
    char input_folder[MAX_PATH_LEN];

    printf("Enter source folder path: ");
    scanf("%s", input_folder);

    printf("Enter file extension to convert (include .): ");
    scanf("%s", from_ext);

    printf("Enter new file extension (include .): ");
    scanf("%s", to_ext);

    snprintf(output_base, MAX_PATH_LEN, "output");
    MKDIR(output_base);

    process_directory(input_folder, output_base);

    printf("Conversion complete. Output saved to ./output\n");

    return 0;
}
