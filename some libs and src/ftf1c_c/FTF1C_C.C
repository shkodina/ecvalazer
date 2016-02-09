/* ftf1c.f -- translated by f2c (version 20000817).
   You must link the resulting object file with the libraries:
        -lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int ftf1c_c(real *are, real *aim, integer *n, integer *in,
        integer *k, real *p)
{
    /* Initialized data */

    static real sys001 = 3.141592f;

    /* System generated locals */
    integer i__1, i__2, i__3, i__4;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    static real a, b, c__;
    static integer i__, j, m;
    static real r__, s, t;
    static integer n1, n2, ii;
    static real co;
    static integer jj, mm;
    static real si;

    /* Parameter adjustments */
    --aim;
    --are;

    /* Function Body */
    n1 = *n / 2;
    mm = n1 / 2;
    n2 = n1 + *k;
    j = *in;
    jj = j;
L1:
    j += *k;
    if (j - n1 <= 0) {
        goto L2;
    } else {
        goto L7;
    }
L2:
    ii = jj + n1;
    r__ = are[j];
    are[j] = are[ii];
    are[ii] = r__;
    r__ = aim[j];
    aim[j] = aim[ii];
    aim[ii] = r__;
    j += *k;
    m = mm;
L3:
    if (jj - m <= 0) {
        goto L5;
    } else {
        goto L4;
    }
L4:
    jj -= m;
    m /= 2;
    goto L3;
L5:
    jj += m;
    if (jj - j <= 0) {
        goto L1;
    } else {
        goto L6;
    }
L6:
    r__ = are[j];
    are[j] = are[jj];
    are[jj] = r__;
    r__ = aim[j];
    aim[j] = aim[jj];
    aim[jj] = r__;
    i__ = j + n2;
    ii = jj + n2;
    r__ = are[i__];
    are[i__] = are[ii];
    are[ii] = r__;
    r__ = aim[i__];
    aim[i__] = aim[ii];
    aim[ii] = r__;
    goto L1;
L7:
    i__ = *k;
    t = sys001;
    if (*p < 0.f) {
        goto L9;
    } else if (*p == 0) {
        goto L13;
    } else {
        goto L8;
    }
L8:
    t = -t;
L9:
    *p = -t;
L10:
    si = 0.f;
    co = 1.f;
    s = (real)sin(t);
    c__ = (real)cos(t);
    t *= .5f;
    ii = i__;
    i__ += i__;
    i__1 = ii;
    i__2 = *k;
    for (m = *in; i__2 < 0 ? m >= i__1 : m <= i__1; m += i__2) {
        i__3 = *n;
        i__4 = i__;
        for (j = m; i__4 < 0 ? j >= i__3 : j <= i__3; j += i__4) {
            jj = j + ii;
            a = are[jj];
            b = aim[jj];
            r__ = a * co - b * si;
            are[jj] = are[j] - r__;
            are[j] += r__;
            r__ = b * co + a * si;
            aim[jj] = aim[j] - r__;
/* L11: */
            aim[j] += r__;
        }
        r__ = c__ * co - s * si;
        si = c__ * si + s * co;
/* L12: */
        co = r__;
    }
    if (i__ - *n >= 0) {
        goto L13;
    } else {
        goto L10;
    }
L13:
    return 0;
} /* ftf1c_c */
