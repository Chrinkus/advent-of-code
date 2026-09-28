#include <stdlib.h>
#include <stdio.h>

enum spells {
        SP_MISSLE,
        SP_DRAIN,
        SP_SHIELD,
        SP_POISON,
        SP_RECHARGE,
        NUM_SPELLS,
};

struct spell {
        int mana;
        int mag;
        int dur;
};

const struct spell spells[NUM_SPELLS] = {
        [SP_MISSLE]     = { .mana =  53, .mag =   4, .dur = 0, },
        [SP_DRAIN]      = { .mana =  73, .mag =   2, .dur = 0, },
        [SP_SHIELD]     = { .mana = 113, .mag =   7, .dur = 6, },
        [SP_POISON]     = { .mana = 173, .mag =   3, .dur = 6, },
        [SP_RECHARGE]   = { .mana = 229, .mag = 101, .dur = 5, },
};

enum buffs {
        BUFF_SHIELD,
        BUFF_RECHARGE,
        NUM_BUFFS,
};

enum debuffs {
        DEBUFF_POISON,
        NUM_DEBUFFS,
};

struct entity {
        int hp;
        int mana;
        int damage;
        int armor;
        int buffs[NUM_BUFFS];
        int debuffs[NUM_DEBUFFS];
};

static void
print_entity(const struct entity* p, const char* name)
{
        printf("Entity: %s\n", name);
        printf("  hp : %d\n", p->hp);
        printf("  mp : %d\n", p->mana);
        printf("  dam: %d\n", p->damage);
        printf("  arm: %d\n", p->armor);
}

static void*
read_boss_data(struct entity* boss)
{
        if (scanf("Hit Points: %d\n", &boss->hp) != 1)
                return NULL;
        if (scanf("Damage: %d\n", &boss->damage) != 1)
                return NULL;

        return boss;
}

static void*
cast_spell(struct entity* self, struct entity* target, int spell, int* cost)
{
        const struct spell* const p = &spells[spell];

        if (self->mana < p->mp_cost)
                return NULL;            // can't cast

        switch (spell) {
        case SP_MISSLE:
                target->hp -= p->mag;
                break;
        case SP_DRAIN:
                target->hp -= p->mag;
                self->hp += p->mag;
                break;
        case SP_SHIELD:
                if (self->buffs[BUFF_SHIELD])
                        return NULL;
                self->buffs[BUFF_SHIELD] = p->dur;
                break;
        case SP_POISON:
                if (target->debuffs[DEBUFF_POISON])
                        return NULL;
                target->debuffs[DEBUFF_POISON] = p->dur;
                break;
        case SP_RECHARGE:
                if (self->buffs[BUFF_RECHARGE])
                        return NULL;
                self->buffs[BUFF_RECHARGE] = p->dur;
                break;
        }

        self->mana -= p->mp_cost;
        *cost += p->mp_cost;

        return self;
}

static void
check_effects(struct entity* e)
{
        for (int i = 0; i < NUM_BUFFS; ++i) {
                if (e->buffs[i] == 0)
                        continue;
                switch (i) {
                case BUFF_SHIELD:
                        e->armor = spells[SP_SHIELD].mag;
                        break;
                case BUFF_RECHARGE:
                        e->mana += spells[SP_RECHARGE].mag;
                        break;
                }
                e->buffs[i] -= 1;
        }

        for (int i = 0; i < NUM_DEBUFFS; ++i) {
                if (e->debuffs[i] == 0)
                        continue;

                switch (i) {
                case DEBUFF_POISON:
                        e->hp -= spells[SP_POISON].mag;
                        break;
                }
                e->debuffs[i] -= 1;
        }
}

static void
take_turn(struct entity* self, struct entity* target)
{
        self->armor = 0;
}

int main(void)
{
        struct entity hero = { .hp = 50, .mana = 500, };

        struct entity boss = { 0 };
        if (!read_boss_data(&boss)) {
                fprintf(stderr, "ERROR: read_boss_data()\n");
                return EXIT_FAILURE;
        }

        print_entity(&hero, "Hero");
        print_entity(&boss, "Boss");

        return EXIT_SUCCESS;
}
