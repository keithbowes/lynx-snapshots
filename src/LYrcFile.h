/*
 * $LynxId: LYrcFile.h,v 1.56 2020/02/23 23:08:55 Keith.Bowes Exp $
 */
#ifndef LYRCFILE_H
#define LYRCFILE_H

#ifndef LYSTRUCTS_H
#include <LYStructs.h>
#endif /* LYSTRUCTS_H */

/* configuration-variable names to share with LYReadCFG.c and LYOptions.c */
#define RC_ACCEPT_ALL_COOKIES           "accept_all_cookies"
#define RC_ALERTSECS                    "alertsecs"
#define RC_ALT_BLAT_MAIL                "alt_blat_mail"
#define RC_ALWAYS_RESUBMIT_POSTS        "always_resubmit_posts"
#define RC_ALWAYS_TRUSTED_EXEC          "always_trusted_exec"
#define RC_ANONFTP_PASSWORD             "anonftp_password"
#define RC_ASSUMED_COLOR                "assumed_color"
#define RC_ASSUMED_DOC_CHARSET_CHOICE   "assumed_doc_charset_choice"
#define RC_ASSUME_CHARSET               "assume_charset"
#define RC_ASSUME_LOCAL_CHARSET         "assume_local_charset"
#define RC_ASSUME_UNREC_CHARSET         "assume_unrec_charset"
#define RC_AUTO_SESSION                 "auto_session"
#define RC_AUTO_UNCACHE_DIRLISTS        "auto_uncache_dirlists"
#define RC_BAD_HTML                     "bad_html"
#define RC_BIBP_BIBHOST                 "bibp_bibhost"
#define RC_BIBP_GLOBALSERVER            "bibp_globalserver"
#define RC_BLAT_MAIL                    "blat_mail"
#define RC_BLOCK_MULTI_BOOKMARKS        "block_multi_bookmarks"
#define RC_BOLD_H1                      "bold_h1"
#define RC_BOLD_HEADERS                 "bold_headers"
#define RC_BOLD_NAME_ANCHORS            "bold_name_anchors"
#define RC_BOOKMARK_FILE                "bookmark_file"
#define RC_BROKEN_FTP_EPSV              "broken_ftp_epsv"
#define RC_BROKEN_FTP_RETR              "broken_ftp_retr"
#define RC_BZIP2_PATH                   "bzip2_path"
#define RC_CASE_SENSITIVE_ALWAYS_ON     "case_sensitive_always_on"
#define RC_CASE_SENSITIVE_SEARCHING     "case_sensitive_searching"
#define RC_CHARACTER_SET                "character_set"
#define RC_CHARSETS_DIRECTORY           "charsets_directory"
#define RC_CHARSET_SWITCH_RULES         "charset_switch_rules"
#define RC_CHECKMAIL                    "checkmail"
#define RC_CHMOD_PATH                   "chmod_path"
#define RC_COLLAPSE_BR_TAGS             "collapse_br_tags"
#define RC_COLOR                        "color"
#define RC_COLOR_STYLE                  "color_style"
#define RC_COMPRESS_PATH                "compress_path"
#define RC_CONNECT_TIMEOUT              "connect_timeout"
#define RC_CONV_JISX0201KANA            "conv_jisx0201kana"
#define RC_COOKIE_ACCEPT_DOMAINS        "cookie_accept_domains"
#define RC_COOKIE_FILE                  "cookie_file"
#define RC_COOKIE_LOOSE_INVALID_DOMAINS "cookie_loose_invalid_domains"
#define RC_COOKIE_QUERY_INVALID_DOMAINS "cookie_query_invalid_domains"
#define RC_COOKIE_REJECT_DOMAINS        "cookie_reject_domains"
#define RC_COOKIE_SAVE_FILE             "cookie_save_file"
#define RC_COOKIE_STRICT_INVALID_DOMAIN "cookie_strict_invalid_domains"
#define RC_COOKIE_VERSION               "cookie_version"
#define RC_COPY_PATH                    "copy_path"
#define RC_CSO_PROXY                    "cso_proxy"
#define RC_CSWING_PATH                  "cswing_path"
#define RC_DEFAULT_BOOKMARK_FILE        "default_bookmark_file"
#define RC_DEFAULT_CACHE_SIZE           "default_cache_size"
#define RC_DEFAULT_COLORS               "default_colors"
#define RC_DEFAULT_EDITOR               "default_editor"
#define RC_DEFAULT_INDEX_FILE           "default_index_file"
#define RC_DEFAULT_KEYPAD_MODE          "default_keypad_mode"
#define RC_DEFAULT_KEYPAD_MODE_NUMARO   "default_keypad_mode_is_numbers_as_arrows"
#define RC_DEFAULT_USER_MODE            "default_user_mode"
#define RC_DEFAULT_VIRTUAL_MEMORY_SIZE  "default_virtual_memory_size"
#define RC_DELAYSECS                    "delaysecs"
#define RC_DIRED_MENU                   "dired_menu"
#define RC_DIR_LIST_ORDER               "dir_list_order"
#define RC_DIR_LIST_STYLE               "dir_list_style"
#define RC_DISPLAY                      "display"
#define RC_DISPLAY_CHARSET_CHOICE       "display_charset_choice"
#define RC_DONT_WRAP_PRE                "dont_wrap_pre"
#define RC_DOWNLOADER                   "downloader"
#define RC_EMACS_KEYS                   "emacs_keys"
#define RC_EMACS_KEYS_ALWAYS_ON         "emacs_keys_always_on"
#define RC_ENABLE_LYNXRC                "enable_lynxrc"
#define RC_ENABLE_SCROLLBACK            "enable_scrollback"
#define RC_EXTERNAL                     "external"
#define RC_EXTERNAL_MENU                "external_menu"
#define RC_FILE_EDITOR                  "file_editor"
#define RC_FILE_SORTING_METHOD          "file_sorting_method"
#define RC_FINGER_PROXY                 "finger_proxy"
#define RC_FOCUS_WINDOW                 "focus_window"
#define RC_FORCE_8BIT_TOUPPER           "force_8bit_toupper"
#define RC_FORCE_COOKIE_PROMPT          "force_cookie_prompt"
#define RC_FORCE_EMPTY_HREFLESS_A       "force_empty_hrefless_a"
#define RC_FORCE_HTML                   "force_html"
#define RC_FORCE_SSL_COOKIES_SECURE     "force_ssl_cookies_secure"
#define RC_FORCE_SSL_PROMPT             "force_ssl_prompt"
#define RC_FORMS_OPTIONS                "forms_options"
#define RC_FTP_FORMAT                   "ftp_format"
#define RC_FTP_PASSIVE                  "ftp_passive"
#define RC_FTP_PROXY                    "ftp_proxy"
#define RC_GLOBAL_EXTENSION_MAP         "global_extension_map"
#define RC_GLOBAL_MAILCAP               "global_mailcap"
#define RC_GOPHER_PROXY                 "gopher_proxy"
#define RC_GOTOBUFFER                   "gotobuffer"
#define RC_GZIP_PATH                    "gzip_path"
#define RC_GUESS_SCHEME                 "guess_scheme"
#define RC_HELPFILE                     "helpfile"
#define RC_HIDDENLINKS                  "hiddenlinks"
#define RC_HIDDEN_LINK_MARKER           "hidden_link_marker"
#define RC_HISTORICAL_COMMENTS          "historical_comments"
#define RC_HTML5_CHARSETS               "html5_charsets"
#define RC_HTMLSRC_ATTRNAME_XFORM       "htmlsrc_attrname_xform"
#define RC_HTMLSRC_TAGNAME_XFORM        "htmlsrc_tagname_xform"
#define RC_HTTPS_PROXY                  "https_proxy"
#define RC_HTTP_PROTOCOL                "http_protocol"
#define RC_HTTP_PROXY                   "http_proxy"
#define RC_INCLUDE                      "include"
#define RC_INFLATE_PATH                 "inflate_path"
#define RC_INFOSECS                     "infosecs"
#define RC_INSTALL_PATH                 "install_path"
#define RC_JUMPBUFFER                   "jumpbuffer"
#define RC_JUMPFILE                     "jumpfile"
#define RC_JUMP_PROMPT                  "jump_prompt"
#define RC_JUSTIFY                      "justify"
#define RC_JUSTIFY_MAX_VOID_PERCENT     "justify_max_void_percent"
#define RC_KBLAYOUT                     "kblayout"
#define RC_KEYBOARD_LAYOUT              "keyboard_layout"
#define RC_KEYMAP                       "keymap"
#define RC_KEYPAD_MODE                  "keypad_mode"
#define RC_LEFTARROW_IN_TEXTFLD_PROMPT  "leftarrow_in_textfield_prompt"
#define RC_LINEEDIT_MODE                "lineedit_mode"
#define RC_LISTONLY                     "listonly"
#define RC_LIST_DECODED                 "list_decoded"
#define RC_LIST_FORMAT                  "list_format"
#define RC_LIST_INLINE                  "list_inline"
#define RC_LIST_NEWS_DATES              "list_news_dates"
#define RC_LIST_NEWS_NUMBERS            "list_news_numbers"
#define RC_LOCALE_CHARSET               "locale_charset"
#define RC_LOCALHOST                    "localhost"
#define RC_LOCALHOST_ALIAS              "localhost_alias"
#define RC_LOCAL_DOMAIN                 "local_domain"
#define RC_LOCAL_EXECUTION_LINKS_ALWAYS "local_execution_links_always_on"
#define RC_LOCAL_EXECUTION_LINKS_LOCAL  "local_execution_links_on_but_not_remote"
#define RC_LYNXCGI_DOCUMENT_ROOT        "lynxcgi_document_root"
#define RC_LYNXCGI_ENVIRONMENT          "lynxcgi_environment"
#define RC_LYNX_HOST_NAME               "lynx_host_name"
#define RC_LYNX_SIG_FILE                "lynx_sig_file"
#define RC_MAIL_ADRS                    "mail_adrs"
#define RC_MAIL_SYSTEM_ERROR_LOGGING    "mail_system_error_logging"
#define RC_MAKE_LINKS_FOR_ALL_IMAGES    "make_links_for_all_images"
#define RC_MAKE_PSEUDO_ALTS_FOR_INLINES "make_pseudo_alts_for_inlines"
#define RC_MAX_COOKIES_BUFFER           "max_cookies_buffer"
#define RC_MAX_COOKIES_DOMAIN           "max_cookies_domain"
#define RC_MAX_COOKIES_GLOBAL           "max_cookies_global"
#define RC_MAX_URI_SIZE                 "max_uri_size"
#define RC_MESSAGESECS                  "messagesecs"
#define RC_MESSAGE_LANGUAGE             "message_language"
#define RC_MINIMAL_COMMENTS             "minimal_comments"
#define RC_MKDIR_PATH                   "mkdir_path"
#define RC_MULTI_BOOKMARK               "multi_bookmark"
#define RC_MULTI_BOOKMARK_SUPPORT       "multi_bookmark_support"
#define RC_MV_PATH                      "mv_path"
#define RC_NCR_IN_BOOKMARKS             "ncr_in_bookmarks"
#define RC_NESTED_TABLES                "nested_tables"
#define RC_NEWSPOST_PROXY               "newspost_proxy"
#define RC_NEWSREPLY_PROXY              "newsreply_proxy"
#define RC_NEWS_CHUNK_SIZE              "news_chunk_size"
#define RC_NEWS_MAX_CHUNK               "news_max_chunk"
#define RC_NEWS_POSTING                 "news_posting"
#define RC_NEWS_PROXY                   "news_proxy"
#define RC_NNTPSERVER                   "nntpserver"
#define RC_NNTP_PROXY                   "nntp_proxy"
#define RC_NONRESTARTING_SIGWINCH       "nonrestarting_sigwinch"
#define RC_NO_DOT_FILES                 "no_dot_files"
#define RC_NO_FILE_REFERER              "no_file_referer"
#define RC_NO_FORCED_CORE_DUMP          "no_forced_core_dump"
#define RC_NO_FROM_HEADER               "no_from_header"
#define RC_NO_ISMAP_IF_USEMAP           "no_ismap_if_usemap"
#define RC_NO_MARGINS                   "no_margins"
#define RC_NO_PAUSE                     "no_pause"
#define RC_NO_PROXY                     "no_proxy"
#define RC_NO_REFERER_HEADER            "no_referer_header"
#define RC_NO_TABLE_CENTER              "no_table_center"
#define RC_NO_TITLE                     "no_title"
#define RC_UPDATE_TERM_TITLE            "update_term_title"
#define RC_NUMBER_FIELDS_ON_LEFT        "number_fields_on_left"
#define RC_NUMBER_LINKS_ON_LEFT         "number_links_on_left"
#define RC_OUTGOING_MAIL_CHARSET        "outgoing_mail_charset"
#define RC_PARTIAL                      "partial"
#define RC_PARTIAL_THRES                "partial_thres"
#define RC_PERSISTENT_COOKIES           "persistent_cookies"
#define RC_PERSONAL_EXTENSION_MAP       "personal_extension_map"
#define RC_PERSONAL_MAILCAP             "personal_mailcap"
#define RC_PERSONAL_MAIL_ADDRESS        "personal_mail_address"
#define RC_PERSONAL_MAIL_NAME           "personal_mail_name"
#define RC_POSITIONABLE_EDITOR          "positionable_editor"
#define RC_PREFERRED_CHARSET            "preferred_charset"
#define RC_PREFERRED_CONTENT_TYPE       "preferred_content_type"
#define RC_PREFERRED_ENCODING           "preferred_encoding"
#define RC_PREFERRED_LANGUAGE           "preferred_language"
#define RC_PREFERRED_MEDIA_TYPES        "preferred_media_types"
#define RC_PREPEND_BASE_TO_SOURCE       "prepend_base_to_source"
#define RC_PREPEND_CHARSET_TO_SOURCE    "prepend_charset_to_source"
#define RC_PRETTYSRC                    "prettysrc"
#define RC_PRETTYSRC_SPEC               "prettysrc_spec"
#define RC_PRETTYSRC_VIEW_NO_ANCHOR_NUM "prettysrc_view_no_anchor_numbering"
#define RC_PRINTER                      "printer"
#define RC_QUIT_DEFAULT_YES             "quit_default_yes"
#define RC_RAW_MODE                     "raw_mode"
#define RC_READ_TIMEOUT                 "read_timeout"
#define RC_REDIRECTION_LIMIT            "redirection_limit"
#define RC_REFERER_WITH_QUERY           "referer_with_query"
#define RC_REPLAYSECS                   "replaysecs"
#define RC_REUSE_TEMPFILES              "reuse_tempfiles"
#define RC_RLOGIN_PATH                  "rlogin_path"
#define RC_RMDIR_PATH                   "rmdir_path"
#define RC_RM_PATH                      "rm_path"
#define RC_RULE                         "rule"
#define RC_RULESFILE                    "rulesfile"
#define RC_RUN_ALL_EXECUTION_LINKS      "run_all_execution_links"
#define RC_RUN_EXECUTION_LINKS_LOCAL    "run_execution_links_on_local_files"
#define RC_SAVE_SPACE                   "save_space"
#define RC_SCAN_FOR_BURIED_NEWS_REFS    "scan_for_buried_news_refs"
#define RC_SCREEN_SIZE                  "screen_size"
#define RC_SCROLLBAR                    "scrollbar"
#define RC_SCROLLBAR_ARROW              "scrollbar_arrow"
#define RC_SEEK_FRAG_AREA_IN_CUR        "seek_frag_area_in_cur"
#define RC_SEEK_FRAG_MAP_IN_CUR         "seek_frag_map_in_cur"
#define RC_SELECT_POPUPS                "select_popups"
#define RC_SEND_USERAGENT               "send_useragent"
#define RC_SESSION_FILE                 "session_file"
#define RC_SESSION_LIMIT                "session_limit"
#define RC_SET_COOKIES                  "set_cookies"
#define RC_SHORT_URL                    "short_url"
#define RC_SHOW_COLOR                   "show_color"
#define RC_SHOW_CURSOR                  "show_cursor"
#define RC_SHOW_DOTFILES                "show_dotfiles"
#define RC_SHOW_KB_NAME                 "show_kb_name"
#define RC_SHOW_KB_RATE                 "show_kb_rate"
#define RC_SNEWSPOST_PROXY              "snewspost_proxy"
#define RC_SNEWSREPLY_PROXY             "snewsreply_proxy"
#define RC_SNEWS_PROXY                  "snews_proxy"
#define RC_SOFT_DQUOTES                 "soft_dquotes"
#define RC_SOURCE_CACHE                 "source_cache"
#define RC_SOURCE_CACHE_FOR_ABORTED     "source_cache_for_aborted"
#define RC_SSL_CERT_FILE                "ssl_cert_file"
#define RC_SSL_CLIENT_CERT_FILE         "ssl_client_cert_file"
#define RC_SSL_CLIENT_KEY_FILE          "ssl_client_key_file"
#define RC_STARTFILE                    "startfile"
#define RC_STATUS_BUFFER_SIZE           "status_buffer_size"
#define RC_STRIP_DOTDOT_URLS            "strip_dotdot_urls"
#define RC_SUBSTITUTE_UNDERSCORES       "substitute_underscores"
#define RC_SUB_BOOKMARKS                "sub_bookmarks"
#define RC_SUFFIX                       "suffix"
#define RC_SUFFIX_ORDER                 "suffix_order"
#define RC_SYSLOG_REQUESTED_URLS        "syslog_requested_urls"
#define RC_SYSLOG_TEXT                  "syslog_text"
#define RC_SYSTEM_EDITOR                "system_editor"
#define RC_SYSTEM_MAIL                  "system_mail"
#define RC_SYSTEM_MAIL_FLAGS            "system_mail_flags"
#define RC_TAGSOUP                      "tagsoup"
#define RC_TAR_PATH                     "tar_path"
#define RC_TELNET_PATH                  "telnet_path"
#define RC_TEXTFIELDS_NEED_ACTIVATION   "textfields_need_activation"
#define RC_TIMEOUT                      "timeout"
#define RC_TN3270_PATH                  "tn3270_path"
#define RC_TOUCH_PATH                   "touch_path"
#define RC_TRACK_INTERNAL_LINKS         "track_internal_links"
#define RC_TRIM_BLANK_LINES             "trim_blank_lines"
#define RC_TRIM_INPUT_FIELDS            "trim_input_fields"
#define RC_TRUSTED_EXEC                 "trusted_exec"
#define RC_TRUSTED_LYNXCGI              "trusted_lynxcgi"
#define RC_UNCOMPRESS_PATH              "uncompress_path"
#define RC_UNDERLINE_LINKS              "underline_links"
#define RC_UNIQUE_URLS                  "unique_urls"
#define RC_UNZIP_PATH                   "unzip_path"
#define RC_UPLOADER                     "uploader"
#define RC_URL_DOMAIN_PREFIXES          "url_domain_prefixes"
#define RC_URL_DOMAIN_SUFFIXES          "url_domain_suffixes"
#define RC_USERAGENT                    "useragent"
#define RC_USER_MODE                    "user_mode"
#define RC_USE_FIXED_RECORDS            "use_fixed_records"
#define RC_USE_MOUSE                    "use_mouse"
#define RC_USE_SELECT_POPUPS            "use_select_popups"
#define RC_UUDECODE_PATH                "uudecode_path"
#define RC_VERBOSE_IMAGES               "verbose_images"
#define RC_VIEWER                       "viewer"
#define RC_VISITED_LINKS                "visited_links"
#define RC_VI_KEYS                      "vi_keys"
#define RC_VI_KEYS_ALWAYS_ON            "vi_keys_always_on"
#define RC_WAIS_PROXY                   "wais_proxy"
#define RC_WAIT_VIEWER_TERMINATION      "wait_viewer_termination"
#define RC_WITH_BACKSPACES              "with_backspaces"
#define RC_XHTML_PARSING                "xhtml_parsing"
#define RC_XLOADIMAGE_COMMAND           "xloadimage_command"
#define RC_ZCAT_PATH                    "zcat_path"
#define RC_ZIP_PATH                     "zip_path"

extern Config_Enum tbl_cookie_version[];
extern Config_Enum tbl_force_prompt[];
extern Config_Enum tbl_keypad_mode[];
extern Config_Enum tbl_multi_bookmarks[];
extern Config_Enum tbl_preferred_content[];
extern Config_Enum tbl_preferred_encoding[];
extern Config_Enum tbl_preferred_media[];
extern Config_Enum tbl_transfer_rate[];
extern Config_Enum tbl_user_mode[];

extern BOOL LYgetEnum(Config_Enum * table, const char *name, int *result);
extern BOOL LYsetRcValue(const char *name, const char *param);
extern BOOL will_save_rc(const char *name);
extern const char *LYputEnum(Config_Enum * table, int value);
extern int enable_lynxrc(char *value);
extern int get_http_protocol(char *value);
extern int get_tagsoup(char *value);
extern int save_rc(FILE *);
extern void read_rc(FILE *);

#endif /* LYRCFILE_H */
