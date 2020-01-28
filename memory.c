#include "kernel.h"

void memman_init(struct MEMMAN far *man)
{
	man->frees = 0;			/* �������̌� */
	man->maxfrees = 0;		/* �󋵊ώ@�p�Ffrees�̍ő�l */
	man->lostsize = 0;		/* ����Ɏ��s�������v�T�C�Y */
	man->losts = 0;			/* ����Ɏ��s������ */
	return;
}

unsigned long memman_total(struct MEMMAN far *man)
/* �����T�C�Y�̍��v��� */
{
	unsigned long i, t = 0;
	for (i = 0; i < man->frees; i++) {
		t += man->free[i].size;
	}
	return t;
}

unsigned long memman_alloc(struct MEMMAN far *man, unsigned long size)
/* �m�� */
{
	unsigned long i, a;
	
	size += 15;
	size &= 0xfff0;
	size >>= 16;
	
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].size >= size) {
			/* �\���ȍL���̂����𔭌� */
			a = man->free[i].addr;
			man->free[i].addr += size;
			man->free[i].size -= size;
			if (man->free[i].size == 0) {
				/* free[i]���Ȃ��Ȃ����̂őO�ւ߂� */
				man->frees--;
				for (; i < man->frees; i++) {
					man->free[i] = man->free[i + 1]; /* �\���̂̑�� */
				}
			}
			return a;
		}
	}
	return 0; /* �������Ȃ� */
}

int memman_free(struct MEMMAN far *man, unsigned long addr, unsigned long size)
/* ��� */
{
	int i, j;
	size += 15;
	size &= 0xfff0;
	size >>= 4;
	/* �܂Ƃ߂₷�����l����ƁAfree[]��addr���ɕ���ł���ق������� */
	/* ������܂��A�ǂ��ɓ����ׂ��������߂� */
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].addr > addr) {
			break;
		}
	}
	/* free[i - 1].addr < addr < free[i].addr */
	if (i > 0) {
		/* �O������ */
		if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
			/* �O�̂����̈�ɂ܂Ƃ߂��� */
			man->free[i - 1].size += size;
			if (i < man->frees) {
				/* �������� */
				if (addr + size == man->free[i].addr) {
					/* �Ȃ�ƌ��Ƃ��܂Ƃ߂��� */
					man->free[i - 1].size += man->free[i].size;
					/* man->free[i]�̍폜 */
					/* free[i]���Ȃ��Ȃ����̂őO�ւ߂� */
					man->frees--;
					for (; i < man->frees; i++) {
						man->free[i] = man->free[i + 1]; /* �\���̂̑�� */
					}
				}
			}
			return 0; /* �����I�� */
		}
	}
	/* �O�Ƃ͂܂Ƃ߂��Ȃ����� */
	if (i < man->frees) {
		/* ��낪���� */
		if (addr + size == man->free[i].addr) {
			/* ���Ƃ͂܂Ƃ߂��� */
			man->free[i].addr = addr;
			man->free[i].size += size;
			return 0; /* �����I�� */
		}
	}
	/* �O�ɂ����ɂ��܂Ƃ߂��Ȃ� */
	if (man->frees < MEMMAN_FREES) {
		/* free[i]�������A���ւ��炵�āA�����܂���� */
		for (j = man->frees; j > i; j--) {
			man->free[j] = man->free[j - 1];
		}
		man->frees++;
		if (man->maxfrees < man->frees) {
			man->maxfrees = man->frees; /* �ő�l���X�V */
		}
		man->free[i].addr = addr;
		man->free[i].size = size;
		return 0; /* �����I�� */
	}
	/* ���ɂ��点�Ȃ����� */
	man->losts++;
	man->lostsize += size;
	return -1; /* ���s�I�� */
}
