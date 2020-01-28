#include "kernel.h"

void memman_init(struct MEMMAN far *man)
{
	man->frees = 0;			/* あき情報の個数 */
	man->maxfrees = 0;		/* 状況観察用：freesの最大値 */
	man->lostsize = 0;		/* 解放に失敗した合計サイズ */
	man->losts = 0;			/* 解放に失敗した回数 */
	return;
}

unsigned long memman_total(struct MEMMAN far *man)
/* あきサイズの合計を報告 */
{
	unsigned long i, t = 0;
	for (i = 0; i < man->frees; i++) {
		t += man->free[i].size;
	}
	return t;
}

unsigned long memman_alloc(struct MEMMAN far *man, unsigned long size)
/* 確保 */
{
	unsigned long i, a;
	
	size += 15;
	size &= 0xfff0;
	size >>= 16;
	
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].size >= size) {
			/* 十分な広さのあきを発見 */
			a = man->free[i].addr;
			man->free[i].addr += size;
			man->free[i].size -= size;
			if (man->free[i].size == 0) {
				/* free[i]がなくなったので前へつめる */
				man->frees--;
				for (; i < man->frees; i++) {
					man->free[i] = man->free[i + 1]; /* 構造体の代入 */
				}
			}
			return a;
		}
	}
	return 0; /* あきがない */
}

int memman_free(struct MEMMAN far *man, unsigned long addr, unsigned long size)
/* 解放 */
{
	int i, j;
	size += 15;
	size &= 0xfff0;
	size >>= 4;
	/* まとめやすさを考えると、free[]がaddr順に並んでいるほうがいい */
	/* だからまず、どこに入れるべきかを決める */
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].addr > addr) {
			break;
		}
	}
	/* free[i - 1].addr < addr < free[i].addr */
	if (i > 0) {
		/* 前がある */
		if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
			/* 前のあき領域にまとめられる */
			man->free[i - 1].size += size;
			if (i < man->frees) {
				/* 後ろもある */
				if (addr + size == man->free[i].addr) {
					/* なんと後ろともまとめられる */
					man->free[i - 1].size += man->free[i].size;
					/* man->free[i]の削除 */
					/* free[i]がなくなったので前へつめる */
					man->frees--;
					for (; i < man->frees; i++) {
						man->free[i] = man->free[i + 1]; /* 構造体の代入 */
					}
				}
			}
			return 0; /* 成功終了 */
		}
	}
	/* 前とはまとめられなかった */
	if (i < man->frees) {
		/* 後ろがある */
		if (addr + size == man->free[i].addr) {
			/* 後ろとはまとめられる */
			man->free[i].addr = addr;
			man->free[i].size += size;
			return 0; /* 成功終了 */
		}
	}
	/* 前にも後ろにもまとめられない */
	if (man->frees < MEMMAN_FREES) {
		/* free[i]より後ろを、後ろへずらして、すきまを作る */
		for (j = man->frees; j > i; j--) {
			man->free[j] = man->free[j - 1];
		}
		man->frees++;
		if (man->maxfrees < man->frees) {
			man->maxfrees = man->frees; /* 最大値を更新 */
		}
		man->free[i].addr = addr;
		man->free[i].size = size;
		return 0; /* 成功終了 */
	}
	/* 後ろにずらせなかった */
	man->losts++;
	man->lostsize += size;
	return -1; /* 失敗終了 */
}
