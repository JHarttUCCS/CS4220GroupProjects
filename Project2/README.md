# Design Goals
- Reliability - critical and lifesaving equipment needs to be able to access the internet at all times
- Security - PII needs to be protected and sensitive equipment needs to be prototected from unauthorized access
  - Seperate the following services and give them their own authentication:
    1. Billing
    2. Medical Records
    3. IT Support
- VLANs - we must seperate general traffic from sensitive traffic
- Mutliple Locations - our network must extend across multiple locations
- Support these types of rooms:
  1. Patient Rooms
  2. Administrative Offices
  3. Laboratories
  4. Radiology Departments
  5. Emergency Rooms
- WAPs: we need a guest WLAN and a staff WLAN.
- Subnetting: We need to efficiently manage our limited address space from 172.16.0.0 to 172.31.255.255.
- HIPAA: try to adhere to the following elements of HIPAA:
  1. Encrypt data at rest (mark it on the network)
  2. Encrypt data in transit (mark it on the network)
- Set up a firewall intrusion detection system
- Implement regular data backups
- Document the network design, especially in these areas:
  1. IP addressing
  2. Server locations
  3. Network Topology
