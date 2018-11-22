int do_update (void)
{
    int ret;
    int   unlocked;
    
    //LOCK_READER: acquire lock as a reader
    // unlocked == 1 means fail, unlocked==0 means success
    unlocked = LOCK_READER (&lock);
    if (unlocked)
        return ERROR;
    
    ret = try_update();
    LOCK_UNLOCK (&lock);
    
    return ret;
}
int try_update (void)
{
    time_t  newtime;
    int  szlen = 0;
    char    bufstr[MAX_STRING];
    int buflen = 0;
    
    newtime = time (NULL);
    if (global_now >= newtime)
        return OK;
    
    /* Fill the buffer with the string representation of current time
     * return number of characters written to the buffer.
     * */
    szlen = get_string_from_time(bufstr, sizeof(bufstr), newtime);
    acquire(global_semaphore); // global_semaphore is a counting semaphore initialized to 10
    buflen = buffer_add (&global_buf, bufstr, szlen);
    release(global_semaphore);
    return buflen;
    
}
/*buffer_t is defined as follows:
 typedef struct
 {
 int size; //maximum length of the buf
 int len;  //current length of the buf
 char* buf; //buffer to store actual string
 } buffer_t;
 */
int
buffer_add (buffer_t *buf, const char *txt, int size)
{
    int free = buf->size - buf->len;
    if (size <= 0)
        return OK;
    if ( free < (size+1)) {
        return ERROR;
    }
    memcpy (buf->buf + buf->len, txt, size);
    buf->len += size;
    buf->buf[buf->len] = '\0';
    
    return OK;
}
