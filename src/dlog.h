/**
 * @file dlog.h
 * @author Kyle Matuszczak (kyle.l.matuszczak@gmail.com)
 * @brief An "improved" fprintf() for debug logging. DLOG() writes a
 *        printf-style message to stderr, prefixed with the calling
 *        function's name and line number. Defining NDEBUG (as with
 *        assert()) compiles every DLOG() call down to nothing, so
 *        release builds pay no cost and don't need dlog.c linked in.
 * @version 0.1
 * @date 2026-08-14
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef DLOG_H
#define DLOG_H

#ifdef NDEBUG

#define DLOG(...) ((void)0)

#else

/**
 * @brief Logs a printf-style debug message to stderr, prefixed with the
 *        calling function name and line number.
 *
 * @param ... printf-style format string, followed by its arguments
 */
#define DLOG(...) dlog_impl(__func__, __LINE__, __VA_ARGS__)

/**
 * @brief Backing implementation for DLOG(). Not called directly; call
 *        through the DLOG() macro so __func__ and __LINE__ capture the
 *        call site instead of this function's own.
 *
 * @param func Calling function name
 * @param line Calling line number
 * @param fmt printf-style format string
 * @param ... Format arguments
 */
void dlog_impl(const char *func, int line, const char *fmt, ...);

#endif /* NDEBUG */

#endif /* DLOG_H */
