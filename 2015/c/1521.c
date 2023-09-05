#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Shop List Database
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct item {
        int cost;
        int damage;
        int armor;
};

enum weapons {
        DAGGER,
        SHORTSWORD,
        WARHAMMER,
        LONGSWORD,
        GREATAXE,
        NUM_WEAPONS,
};

const struct item weapons[NUM_WEAPONS] = {
        [DAGGER]        = { .cost = 8,  .damage = 4,    .armor = 0, },
        [SHORTSWORD]    = { .cost = 10, .damage = 5,    .armor = 0, },
        [WARHAMMER]     = { .cost = 25, .damage = 6,    .armor = 0, },
        [LONGSWORD]     = { .cost = 40, .damage = 7,    .armor = 0, },
        [GREATAXE]      = { .cost = 74, .damage = 8,    .armor = 0, },
};

enum armors {
        NO_ARMOR,
        LEATHER,
        CHAINMAIL,
        SPLINTMAIL,
        BANDEDMAIL,
        PLATEMAIL,
        NUM_ARMORS,
};

const struct item armors[NUM_ARMORS] = {
        [LEATHER]       = { .cost = 13,  .damage = 0,   .armor = 1, },
        [CHAINMAIL]     = { .cost = 31,  .damage = 0,   .armor = 2, },
        [SPLINTMAIL]    = { .cost = 53,  .damage = 0,   .armor = 3, },
        [BANDEDMAIL]    = { .cost = 75,  .damage = 0,   .armor = 4, },
        [PLATEMAIL]     = { .cost = 102, .damage = 0,   .armor = 5, },
};

enum rings {
        NO_RING,
        DAMAGE_1,
        DAMAGE_2,
        DAMAGE_3,
        DEFENCE_1,
        DEFENCE_2,
        DEFENCE_3,
        NUM_RINGS,
};

const struct item rings[NUM_RINGS] = {
        [DAMAGE_1]      = { .cost = 25,  .damage = 1,   .armor = 0, },
        [DAMAGE_2]      = { .cost = 50,  .damage = 2,   .armor = 0, },
        [DAMAGE_3]      = { .cost = 100, .damage = 3,   .armor = 0, },
        [DEFENCE_1]     = { .cost = 20,  .damage = 0,   .armor = 1, },
        [DEFENCE_2]     = { .cost = 40,  .damage = 0,   .armor = 2, },
        [DEFENCE_3]     = { .cost = 80,  .damage = 0,   .armor = 3, },
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Solution
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum outcomes { GAME_OVER, VICTORY, };

enum gear_slots {
        GS_WEAPON,
        GS_ARMOR,
        GS_RING_1,
        GS_RING_2,
        NUM_GEAR_SLOTS,
};

struct player {
        int hp;
        int damage;
        int armor;
        int gear[NUM_GEAR_SLOTS];
        int gear_value;
};

struct npc {
        int hp;
        int damage;
        int armor;
};

struct costs {
        int most;
        int least;
};

static void*
read_boss_data(struct npc* boss)
{
        if (scanf("Hit Points: %d\n", &boss->hp) != 1)
                return NULL;
        if (scanf("Damage: %d\n", &boss->damage) != 1)
                return NULL;
        if (scanf("Armor: %d\n", &boss->armor) != 1)
                return NULL;

        return boss;
}

static int
sim_battle(struct player hero, struct npc boss)
{
        const int hero_dam = hero.damage > boss.armor
                ? hero.damage - boss.armor : 1;
        const int boss_dam = boss.damage > hero.armor
                ? boss.damage - hero.armor : 1;

        while (1) {
                boss.hp -= hero_dam;
                if (boss.hp <= 0)
                        return VICTORY;

                hero.hp -= boss_dam;
                if (hero.hp <= 0)
                        return GAME_OVER;
        }
}

static void
sim_battle_and_track_costs(struct player* hero, const struct npc* boss,
                struct costs* costs)
{
        switch (sim_battle(*hero, *boss)) {
        case VICTORY:
                if (hero->gear_value < costs->least)
                        costs->least = hero->gear_value;
                break;
        case GAME_OVER:
                if (hero->gear_value > costs->most)
                        costs->most = hero->gear_value;
                break;
        }
}

static void
set_player_stats(struct player* hero)
{
        hero->hp = 100;
        hero->gear_value = 0;

        const struct item* p = &weapons[hero->gear[GS_WEAPON]];
        hero->damage = p->damage;
        hero->gear_value += p->cost;

        p = &armors[hero->gear[GS_ARMOR]];
        hero->armor = p->armor;
        hero->gear_value += p->cost;

        p = &rings[hero->gear[GS_RING_1]];
        hero->damage += p->damage;
        hero->armor += p->armor;
        hero->gear_value += p->cost;

        p = &rings[hero->gear[GS_RING_2]];
        hero->damage += p->damage;
        hero->armor += p->armor;
        hero->gear_value += p->cost;
}

static void
sim_all_equip_sets(struct player* hero, const struct npc* boss,
                struct costs* costs)
{
        for (int i = 0; i < NUM_WEAPONS; ++i) {
                hero->gear[GS_WEAPON] = i;
                for (int j = 0; j < NUM_ARMORS; ++j) {
                        hero->gear[GS_ARMOR] = j;
                        for (int k = 0; k < NUM_RINGS; ++k) {
                                hero->gear[GS_RING_1] = k;
                                for (int l = 0; l < NUM_RINGS; ++l) {
                                        // skip duplicate rings
                                        if (k == l && k != 0)
                                                continue;

                                        hero->gear[GS_RING_2] = l;
                                        set_player_stats(hero);

                                        sim_battle_and_track_costs(hero, boss,
                                                        costs);
                                }
                        }
                }
        }
}

int main(void)
{
        struct player hero = { 0 };
        struct npc boss = { 0 };
        if (!read_boss_data(&boss)) {
                fprintf(stderr, "read_boss_data()\n");
                return EXIT_FAILURE;
        }

        struct costs costs = { .most = INT_MIN, .least = INT_MAX, };
        sim_all_equip_sets(&hero, &boss, &costs);

        printf("%d\n", costs.least);
        printf("%d\n", costs.most);

        return EXIT_SUCCESS;
}
