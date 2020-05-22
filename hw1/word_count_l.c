/*
 * Implementation of the word_count interface using Pintos lists.
 *
 * You may modify this file, and are expected to modify it.
 */

/*
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif

#include "word_count.h"

void init_words(word_count_list_t *wclist) {
  list_init(wclist);
}

size_t len_words(word_count_list_t *wclist) {
  return list_size(wclist);
}

word_count_t *find_word(word_count_list_t *wclist, char *word) {
  /* Walk through the whole list to find the word. */
  struct list_elem *e;
  word_count_t *wc;
  for (e = list_begin(wclist); e != list_end(wclist);
       e = list_next(e)) {
    wc = list_entry(e, word_count_t, elem);
    if (strcmp(wc -> word, word) == 0) {
      return wc;
    }
  } 
  return NULL;
}

word_count_t *add_word_with_count(word_count_list_t *wclist, char *word,
                                  int count) {
  /*
   * If word is present in word_counts list, increment the count.
   * Otherwise, insert at head of list with count COUNT.
   */
  word_count_t *wc = find_word(wclist, word);
  if (wc != NULL) {
    wc -> count += count;
  } else if ((wc = malloc(sizeof(word_count_t))) != NULL) {
    wc -> word = word;
    wc -> count = count;
    list_push_front(wclist, &(wc -> elem));
  } else {
    perror("malloc");
  }
  return wc;
}

word_count_t *add_word(word_count_list_t *wclist, char *word) {
  return add_word_with_count(wclist, word, 1);
}

void fprint_words(word_count_list_t *wclist, FILE *outfile) {
  struct list_elem *e;
  word_count_t *wc;
  for (e = list_begin(wclist); e != list_end(wclist);
       e = list_next(e)) {
    wc = list_entry(e, word_count_t, elem);
    fprintf(outfile, "%8d\t%s\n", wc -> count, wc -> word);
  }
}

static bool less_list(const struct list_elem *ewc1,
                      const struct list_elem *ewc2, void *aux) {
  bool (*is_less)(word_count_t *, word_count_t *) = aux;
  return is_less(list_entry(ewc1, word_count_t, elem), list_entry(ewc2, word_count_t, elem));
}

void wordcount_sort(word_count_list_t *wclist,
                    bool less(const word_count_t *, const word_count_t *)) {
  list_sort(wclist, less_list, less);
}
