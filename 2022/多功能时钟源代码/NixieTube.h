unsigned char NixieTubeTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
// ��Ӧ0~9

void NixieTube(unsigned char location, unsigned char number)
{
    // λѡ
    switch (location)
    {
    // LED8 �Ǵ������ҵĵ�һ�������
    case 1:
        P2_4 = 1;
        P2_3 = 1;
        P2_2 = 1;
        break;
    case 2:
        P2_4 = 1;
        P2_3 = 1;
        P2_2 = 0;
        break;
    case 3:
        P2_4 = 1;
        P2_3 = 0;
        P2_2 = 1;
        break;
    case 4:
        P2_4 = 1;
        P2_3 = 0;
        P2_2 = 0;
        break;
    case 5:
        P2_4 = 0;
        P2_3 = 1;
        P2_2 = 1;
        break;
    case 6:
        P2_4 = 0;
        P2_3 = 1;
        P2_2 = 0;
        break;
    case 7:
        P2_4 = 0;
        P2_3 = 0;
        P2_2 = 1;
        break;
    case 8:
        P2_4 = 0;
        P2_3 = 0;
        P2_2 = 0;
        break;
    }

    // ��ѡ
    P0 = NixieTubeTable[number];
    Delay1ms(1); // ��ʱ1ms �������ȶ���ʾ
    P0 = 0x00;   // ����
}