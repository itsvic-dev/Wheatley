#include "sys/apic.h"
#include <panic.h>
#include <printf.h>
#include <sys/idt.h>
#include <sys/isr.h>

void isr_write(void) {
  printf("isr: setting ISRs\n");

  idt_set_handler(0, 0, isr0);
  idt_set_handler(1, 1, isr1);
  idt_set_handler(2, 0, isr2);
  idt_set_handler(3, 0, isr3);
  idt_set_handler(4, 0, isr4);
  idt_set_handler(5, 0, isr5);
  idt_set_handler(6, 0, isr6);
  idt_set_handler(7, 0, isr7);
  idt_set_handler(8, 1, isr8);
  idt_set_handler(9, 0, isr9);
  idt_set_handler(10, 0, isr10);
  idt_set_handler(11, 0, isr11);
  idt_set_handler(12, 0, isr12);
  idt_set_handler(13, 0, isr13);
  idt_set_handler(14, 0, isr14);
  idt_set_handler(15, 0, isr15);
  idt_set_handler(16, 0, isr16);
  idt_set_handler(17, 0, isr17);
  idt_set_handler(18, 1, isr18);
  idt_set_handler(19, 0, isr19);
  idt_set_handler(20, 0, isr20);
  idt_set_handler(21, 0, isr21);
  idt_set_handler(22, 0, isr22);
  idt_set_handler(23, 0, isr23);
  idt_set_handler(24, 0, isr24);
  idt_set_handler(25, 0, isr25);
  idt_set_handler(26, 0, isr26);
  idt_set_handler(27, 0, isr27);
  idt_set_handler(28, 0, isr28);
  idt_set_handler(29, 0, isr29);
  idt_set_handler(30, 0, isr30);
  idt_set_handler(31, 0, isr31);
  idt_set_handler(32, 0, isr32);
  idt_set_handler(33, 0, isr33);
  idt_set_handler(34, 0, isr34);
  idt_set_handler(35, 0, isr35);
  idt_set_handler(36, 0, isr36);
  idt_set_handler(37, 0, isr37);
  idt_set_handler(38, 0, isr38);
  idt_set_handler(39, 0, isr39);
  idt_set_handler(40, 0, isr40);
  idt_set_handler(41, 0, isr41);
  idt_set_handler(42, 0, isr42);
  idt_set_handler(43, 0, isr43);
  idt_set_handler(44, 0, isr44);
  idt_set_handler(45, 0, isr45);
  idt_set_handler(46, 0, isr46);
  idt_set_handler(47, 0, isr47);
  idt_set_handler(48, 0, isr48);
  idt_set_handler(49, 0, isr49);
  idt_set_handler(50, 0, isr50);
  idt_set_handler(51, 0, isr51);
  idt_set_handler(52, 0, isr52);
  idt_set_handler(53, 0, isr53);
  idt_set_handler(54, 0, isr54);
  idt_set_handler(55, 0, isr55);
  idt_set_handler(56, 0, isr56);
  idt_set_handler(57, 0, isr57);
  idt_set_handler(58, 0, isr58);
  idt_set_handler(59, 0, isr59);
  idt_set_handler(60, 0, isr60);
  idt_set_handler(61, 0, isr61);
  idt_set_handler(62, 0, isr62);
  idt_set_handler(63, 0, isr63);
  idt_set_handler(64, 0, isr64);
  idt_set_handler(65, 0, isr65);
  idt_set_handler(66, 0, isr66);
  idt_set_handler(67, 0, isr67);
  idt_set_handler(68, 0, isr68);
  idt_set_handler(69, 0, isr69);
  idt_set_handler(70, 0, isr70);
  idt_set_handler(71, 0, isr71);
  idt_set_handler(72, 0, isr72);
  idt_set_handler(73, 0, isr73);
  idt_set_handler(74, 0, isr74);
  idt_set_handler(75, 0, isr75);
  idt_set_handler(76, 0, isr76);
  idt_set_handler(77, 0, isr77);
  idt_set_handler(78, 0, isr78);
  idt_set_handler(79, 0, isr79);
  idt_set_handler(80, 0, isr80);
  idt_set_handler(81, 0, isr81);
  idt_set_handler(82, 0, isr82);
  idt_set_handler(83, 0, isr83);
  idt_set_handler(84, 0, isr84);
  idt_set_handler(85, 0, isr85);
  idt_set_handler(86, 0, isr86);
  idt_set_handler(87, 0, isr87);
  idt_set_handler(88, 0, isr88);
  idt_set_handler(89, 0, isr89);
  idt_set_handler(90, 0, isr90);
  idt_set_handler(91, 0, isr91);
  idt_set_handler(92, 0, isr92);
  idt_set_handler(93, 0, isr93);
  idt_set_handler(94, 0, isr94);
  idt_set_handler(95, 0, isr95);
  idt_set_handler(96, 0, isr96);
  idt_set_handler(97, 0, isr97);
  idt_set_handler(98, 0, isr98);
  idt_set_handler(99, 0, isr99);
  idt_set_handler(100, 0, isr100);
  idt_set_handler(101, 0, isr101);
  idt_set_handler(102, 0, isr102);
  idt_set_handler(103, 0, isr103);
  idt_set_handler(104, 0, isr104);
  idt_set_handler(105, 0, isr105);
  idt_set_handler(106, 0, isr106);
  idt_set_handler(107, 0, isr107);
  idt_set_handler(108, 0, isr108);
  idt_set_handler(109, 0, isr109);
  idt_set_handler(110, 0, isr110);
  idt_set_handler(111, 0, isr111);
  idt_set_handler(112, 0, isr112);
  idt_set_handler(113, 0, isr113);
  idt_set_handler(114, 0, isr114);
  idt_set_handler(115, 0, isr115);
  idt_set_handler(116, 0, isr116);
  idt_set_handler(117, 0, isr117);
  idt_set_handler(118, 0, isr118);
  idt_set_handler(119, 0, isr119);
  idt_set_handler(120, 0, isr120);
  idt_set_handler(121, 0, isr121);
  idt_set_handler(122, 0, isr122);
  idt_set_handler(123, 0, isr123);
  idt_set_handler(124, 0, isr124);
  idt_set_handler(125, 0, isr125);
  idt_set_handler(126, 0, isr126);
  idt_set_handler(127, 0, isr127);
  idt_set_handler(128, 0, isr128);
  idt_set_handler(129, 0, isr129);
  idt_set_handler(130, 0, isr130);
  idt_set_handler(131, 0, isr131);
  idt_set_handler(132, 0, isr132);
  idt_set_handler(133, 0, isr133);
  idt_set_handler(134, 0, isr134);
  idt_set_handler(135, 0, isr135);
  idt_set_handler(136, 0, isr136);
  idt_set_handler(137, 0, isr137);
  idt_set_handler(138, 0, isr138);
  idt_set_handler(139, 0, isr139);
  idt_set_handler(140, 0, isr140);
  idt_set_handler(141, 0, isr141);
  idt_set_handler(142, 0, isr142);
  idt_set_handler(143, 0, isr143);
  idt_set_handler(144, 0, isr144);
  idt_set_handler(145, 0, isr145);
  idt_set_handler(146, 0, isr146);
  idt_set_handler(147, 0, isr147);
  idt_set_handler(148, 0, isr148);
  idt_set_handler(149, 0, isr149);
  idt_set_handler(150, 0, isr150);
  idt_set_handler(151, 0, isr151);
  idt_set_handler(152, 0, isr152);
  idt_set_handler(153, 0, isr153);
  idt_set_handler(154, 0, isr154);
  idt_set_handler(155, 0, isr155);
  idt_set_handler(156, 0, isr156);
  idt_set_handler(157, 0, isr157);
  idt_set_handler(158, 0, isr158);
  idt_set_handler(159, 0, isr159);
  idt_set_handler(160, 0, isr160);
  idt_set_handler(161, 0, isr161);
  idt_set_handler(162, 0, isr162);
  idt_set_handler(163, 0, isr163);
  idt_set_handler(164, 0, isr164);
  idt_set_handler(165, 0, isr165);
  idt_set_handler(166, 0, isr166);
  idt_set_handler(167, 0, isr167);
  idt_set_handler(168, 0, isr168);
  idt_set_handler(169, 0, isr169);
  idt_set_handler(170, 0, isr170);
  idt_set_handler(171, 0, isr171);
  idt_set_handler(172, 0, isr172);
  idt_set_handler(173, 0, isr173);
  idt_set_handler(174, 0, isr174);
  idt_set_handler(175, 0, isr175);
  idt_set_handler(176, 0, isr176);
  idt_set_handler(177, 0, isr177);
  idt_set_handler(178, 0, isr178);
  idt_set_handler(179, 0, isr179);
  idt_set_handler(180, 0, isr180);
  idt_set_handler(181, 0, isr181);
  idt_set_handler(182, 0, isr182);
  idt_set_handler(183, 0, isr183);
  idt_set_handler(184, 0, isr184);
  idt_set_handler(185, 0, isr185);
  idt_set_handler(186, 0, isr186);
  idt_set_handler(187, 0, isr187);
  idt_set_handler(188, 0, isr188);
  idt_set_handler(189, 0, isr189);
  idt_set_handler(190, 0, isr190);
  idt_set_handler(191, 0, isr191);
  idt_set_handler(192, 0, isr192);
  idt_set_handler(193, 0, isr193);
  idt_set_handler(194, 0, isr194);
  idt_set_handler(195, 0, isr195);
  idt_set_handler(196, 0, isr196);
  idt_set_handler(197, 0, isr197);
  idt_set_handler(198, 0, isr198);
  idt_set_handler(199, 0, isr199);
  idt_set_handler(200, 0, isr200);
  idt_set_handler(201, 0, isr201);
  idt_set_handler(202, 0, isr202);
  idt_set_handler(203, 0, isr203);
  idt_set_handler(204, 0, isr204);
  idt_set_handler(205, 0, isr205);
  idt_set_handler(206, 0, isr206);
  idt_set_handler(207, 0, isr207);
  idt_set_handler(208, 0, isr208);
  idt_set_handler(209, 0, isr209);
  idt_set_handler(210, 0, isr210);
  idt_set_handler(211, 0, isr211);
  idt_set_handler(212, 0, isr212);
  idt_set_handler(213, 0, isr213);
  idt_set_handler(214, 0, isr214);
  idt_set_handler(215, 0, isr215);
  idt_set_handler(216, 0, isr216);
  idt_set_handler(217, 0, isr217);
  idt_set_handler(218, 0, isr218);
  idt_set_handler(219, 0, isr219);
  idt_set_handler(220, 0, isr220);
  idt_set_handler(221, 0, isr221);
  idt_set_handler(222, 0, isr222);
  idt_set_handler(223, 0, isr223);
  idt_set_handler(224, 0, isr224);
  idt_set_handler(225, 0, isr225);
  idt_set_handler(226, 0, isr226);
  idt_set_handler(227, 0, isr227);
  idt_set_handler(228, 0, isr228);
  idt_set_handler(229, 0, isr229);
  idt_set_handler(230, 0, isr230);
  idt_set_handler(231, 0, isr231);
  idt_set_handler(232, 0, isr232);
  idt_set_handler(233, 0, isr233);
  idt_set_handler(234, 0, isr234);
  idt_set_handler(235, 0, isr235);
  idt_set_handler(236, 0, isr236);
  idt_set_handler(237, 0, isr237);
  idt_set_handler(238, 0, isr238);
  idt_set_handler(239, 0, isr239);
  idt_set_handler(240, 0, isr240);
  idt_set_handler(241, 0, isr241);
  idt_set_handler(242, 0, isr242);
  idt_set_handler(243, 0, isr243);
  idt_set_handler(244, 0, isr244);
  idt_set_handler(245, 0, isr245);
  idt_set_handler(246, 0, isr246);
  idt_set_handler(247, 0, isr247);
  idt_set_handler(248, 0, isr248);
  idt_set_handler(249, 0, isr249);
  idt_set_handler(250, 0, isr250);
  idt_set_handler(251, 0, isr251);
  idt_set_handler(252, 0, isr252);
  idt_set_handler(253, 0, isr253);
  idt_set_handler(254, 0, isr254);
  idt_set_handler(255, 0, isr255);

  idt_reload();
}

static event_handler_t event_handlers[256] = {NULL};
static const char isr_exceptions[32][35] = {
    "Divide Error (#DE)",
    "Debug Exception (#DB)",
    "NMI Interrupt",
    "Breakpoint (#BP)",
    "Overflow (#OF)",
    "BOUND Range Exceeded (#BR)",
    "Invalid Opcode (#UD)",
    "No Math Coprocessor (#NM)",
    "Double Fault (#DF)",
    "Reserved",
    "Invalid TSS (#TS)",
    "Segment Not Present (#NP)",
    "Stack-Segment Fault (#SS)",
    "General Protection (#GP)",
    "Page Fault (#PF)",
    "Reserved",
    "x87 Floating-Point Error (#MF)",
    "Alignment Check (#AC)",
    "Machine Check (#MC)",
    "SIMD Floating-Point Exception (#XM)",
    "Virtualization Exception (#VE)",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"};

void isr_register_handler(int n, void *handler) {
  if (n >= 256)
    return;
  event_handlers[n] = handler;
}

void isr_handle(registers_t *r) {
  if (r->isrNumber < 256 && event_handlers[r->isrNumber] != NULL)
    return event_handlers[r->isrNumber](r);

  if (r->isrNumber < 32) {
    printf("isr: Unhandled exception: %s\n", isr_exceptions[r->isrNumber]);
    panic("Unhandled exception", r);
  }

  printf("isr: Unhandled ISR: %d\n", r->isrNumber);
  apic_eoi();
}
