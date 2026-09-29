#!/bin/sh
# Выход клиентов туннеля в интернет. Стенд и waystation уже должны быть подняты.
set -e

EXT=$(ip -4 route get 8.8.8.8 | awk '{ for (i = 1; i <= NF; i++) if ($i == "dev") { print $(i + 1); exit } }')

if [ -z "$EXT" ]; then
    echo "не вижу интерфейс до 8.8.8.8" >&2
    exit 1
fi

# Запоминаю имя, чтобы nat_down снял те же правила
echo "$EXT" > /run/waystation_nat_ext

sysctl -w net.ipv4.ip_forward=1

if ip link show ws0 >/dev/null 2>&1; then
    sysctl -w net.ipv4.conf.ws0.send_redirects=0
fi

iptables -t nat -C POSTROUTING -s 10.0.0.0/24 -o "$EXT" -j MASQUERADE 2>/dev/null || \
    iptables -t nat -I POSTROUTING 1 -s 10.0.0.0/24 -o "$EXT" -j MASQUERADE

iptables -C FORWARD -i ws0 -o "$EXT" -j ACCEPT 2>/dev/null || \
    iptables -I FORWARD 1 -i ws0 -o "$EXT" -j ACCEPT
iptables -C FORWARD -i "$EXT" -o ws0 -m state --state RELATED,ESTABLISHED -j ACCEPT 2>/dev/null || \
    iptables -I FORWARD 1 -i "$EXT" -o ws0 -m state --state RELATED,ESTABLISHED -j ACCEPT

if ip netns list | grep -q '^wsc'; then
    ip netns exec wsc ip route replace default via 10.0.0.1
fi
echo "NAT для 10.0.0.0/24 через $EXT"
