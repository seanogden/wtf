/* Simple program to test using HyperDex as inode map for a filesystem
 *
 * Set up:
 * echo 'space wtf key path attributes map(string, string) blockmap' | hyperdex add-space -h 127.0.0.1 -p 1982
 */

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

void fusewtf_initialize();
void fusewtf_destroy();

int fusewtf_search(const char* value, const char** one_result);
int fusewtf_search_exists(const char* value);

void fusewtf_loop();

int fusewtf_read(const char** output_filename);

#ifdef __cplusplus
}
#endif //__cplusplus
