from mininet.topo import Topo
from mininet.link import TCLink
from mininet.util import irange

class Challenge4Topo( Topo ):

	def __init__( self ):
		# Initialize topology
		Topo.__init__( self )
		hosts = [None] * 20

		client = self.addHost('client')
		sw0 = self.addSwitch('s0')
		sw1 = self.addSwitch('s1')
		sw2 = self.addSwitch('s2')
		self.addLink(client, sw0)
		self.addLink(sw0, sw1)
		self.addLink(sw0, sw2)

		for i in irange(1, 10):
			hosts[i] = self.addHost('h%s' % i)

		for i in irange(1, 5):
			self.addLink(sw1, hosts[i])
			self.addLink(sw2, hosts[i + 5])

topos = { 'challenge4_topo': ( lambda: Challenge4Topo() ) }
