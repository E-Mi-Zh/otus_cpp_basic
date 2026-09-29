#!/bin/sh
# Снимает NAT

EXT=$(cat /run/waystation_nat_ext 2>/dev/null || true)

if [ -n "$EXT" ]; then
    iptables -t nat -D POSTROUTING -s 10.0.0.0/24 -o "$EXT" -j MASQUERADE 2>/dev/null || true
    iptables -D FORWARD -i ws0 -o "$EXT" -j ACCEPT 2>/dev/null || true
    iptables -D FORWARD -i "$EXT" -o ws0 -m state --state RELATED,ESTABLISHED -j ACCEPT 2>/dev/null || true
    rm -f /run/waystation_nat_ext
fi

ip netns exec wsc ip route del default via 10.0.0.1 2>/dev/null || true
echo "NAT снят."
