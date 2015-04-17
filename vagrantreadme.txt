INSTALLEREN
 - Aangeleverde Vagrant en VirtualBox installeren: gewoon de bestanden uitvoeren.

UITVOEREN
 - VirtualBox starten (stopt foutmeldingen van Vagrant, hoeft maar één keer [technisch gezien]);
 - CMD openen in de folder waar het 'Vagrantfile' staat, voor ons deze folder (DudeServer repo);
	= Voer 'vagrant up' uit; dit start Vagrant met de instellingen die in het 'Vagrantfile' staan. 
		Als dit de eerste keer is, dan moet hij zeer waarschijnlijk Ubuntu 14.04 Trusty downloaden. Dit kan even duren, afhankelijk van je internetsnelheid. 
		Zodra dit klaar is, start hij hem automatisch op in VirtualBox.
	= Voer 'vagrant ssh' uit; hiermee zet je een SSH-sessie op in CMD naar je Ubuntu instantie en ben je ermee verbonden, zodat je ermee kan communiceren.
 - Voer je activiteiten uit die samen met Vagrant gaan, b.v. programmeren e.d. Zie dat alle veranderingen die je aan de folders en bestanden aanbrengt, dat die ook in 
	de Ubuntu omgeving worden uitgevoerd. Probeer maar eens 'ls /vagrant' nadat je in de folder van het 'Vagrantfile' staat een bestand heb toevoegd.
 - Zodra je klaar bent met de SSH sessie, voer je 'exit' uit. Dit sluit de SSH sessie.

STOPPEN
 - Zodra je klaar bent met Vagrant, voer je 'vagrant destroy' uit. Dit 'vernietigt' de virtuele machine. Don't fret, though! Alle instellingen bestaan nog. Gebruik de '-f' flag om confirmatie te omzijlen.
	= Wil je de virtuele machine in slaapstand zetten? Gebruik dan 'vagrant suspend' om hem in slaapstand te zetten, en 'vagrant up' of 'vagrant resume' om hem weer op te starten.
	= Wil je de virtuele machine uitzetten? Gebruik dan 'vagrant halt' om hem uit te zetten. Hij bestaat nog wel.