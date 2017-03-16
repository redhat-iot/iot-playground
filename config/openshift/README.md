The ansible-hosts.cfg configures the OpenShift installation of the iot playground. 

Before deploying OpenShift, ensure the [prerequisites](https://docs.openshift.org/latest/install_config/install/prerequisites.html#install-config-install-prerequisites)
are met. To deploy it, you need to install [ansible](https://www.ansible.com/) and clone the [playbooks](https://github.com/openshift/openshift-ansible).

Once you have the playbooks, setting up the cluster is simple:

    ansible-playbook -u <user> -k -i ansible-hosts.cfg openshift-ansible/playbooks/byo/config.yml

Once this playbook has run, you need to login to the host and add users using htpasswd. To do that,
you need to install the httpd-tools package

    yum install httpd-tools

Then, create the initial password file with your desired user

    htpasswd -c /etc/origin/master/htpasswd iot

You should be able to login with that user on the openshift console.

TODO: Setup certificates
