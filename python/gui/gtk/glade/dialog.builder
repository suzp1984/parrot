<?xml version="1.0"?>
<interface>
  <requires lib="gtk+" version="2.14"/>
  <!-- interface-naming-policy project-wide -->
  <object class="GtkWindow" id="window1">
    <signal name="destroy" handler="gtk_main_quit"/>
    <signal name="delete_event" handler="cb_delete_event"/>
    <child>
      <object class="GtkButton" id="About">
        <property name="label" translatable="yes">gtk-about</property>
        <property name="visible">True</property>
        <property name="can_focus">True</property>
        <property name="receives_default">True</property>
        <property name="use_stock">True</property>
        <signal name="clicked" handler="cb_show_about"/>
      </object>
    </child>
  </object>
  <object class="GtkDialog" id="dialog1">
    <property name="border_width">5</property>
    <property name="modal">True</property>
    <property name="type_hint">normal</property>
    <property name="transient_for">window1</property>
    <property name="has_separator">False</property>
    <child internal-child="vbox">
      <object class="GtkVBox" id="dialog-vbox1">
        <property name="visible">True</property>
        <property name="spacing">2</property>
        <child>
          <object class="GtkLabel" id="label1">
            <property name="visible">True</property>
            <property name="label" translatable="yes">Are you sure you want to quit?</property>
          </object>
          <packing>
            <property name="position">1</property>
          </packing>
        </child>
        <child internal-child="action_area">
          <object class="GtkHButtonBox" id="dialog-action_area1">
            <property name="visible">True</property>
            <property name="layout_style">end</property>
            <child>
              <object class="GtkButton" id="button1">
                <property name="label" translatable="yes">gtk-yes</property>
                <property name="visible">True</property>
                <property name="can_focus">True</property>
                <property name="receives_default">True</property>
                <property name="use_stock">True</property>
              </object>
              <packing>
                <property name="position">0</property>
              </packing>
            </child>
            <child>
              <object class="GtkButton" id="button2">
                <property name="label" translatable="yes">gtk-no</property>
                <property name="visible">True</property>
                <property name="can_focus">True</property>
                <property name="receives_default">True</property>
                <property name="use_stock">True</property>
              </object>
              <packing>
                <property name="position">1</property>
              </packing>
            </child>
          </object>
          <packing>
            <property name="expand">False</property>
            <property name="pack_type">end</property>
            <property name="position">0</property>
          </packing>
        </child>
      </object>
    </child>
    <action-widgets>
      <action-widget response="1">button1</action-widget>
      <action-widget response="2">button2</action-widget>
    </action-widgets>
  </object>
  <object class="GtkAboutDialog" id="aboutdialog1">
    <property name="border_width">5</property>
    <property name="type_hint">normal</property>
    <property name="has_separator">False</property>
    <property name="program_name">Sample Dialog App</property>
    <property name="version">0.2</property>
    <property name="copyright" translatable="yes">Copyright 2009 Tadej Borov&#x161;ak</property>
    <property name="website">http://tadeboro.blogspot.com</property>
    <property name="authors">Tadej Borov&#x161;ak &lt;tadeboro@gmail.com&gt;</property>
    <child internal-child="vbox">
      <object class="GtkVBox" id="dialog-vbox2">
        <property name="visible">True</property>
        <property name="spacing">2</property>
        <child>
          <placeholder/>
        </child>
        <child internal-child="action_area">
          <object class="GtkHButtonBox" id="dialog-action_area2">
            <property name="visible">True</property>
            <property name="layout_style">end</property>
          </object>
          <packing>
            <property name="expand">False</property>
            <property name="pack_type">end</property>
            <property name="position">0</property>
          </packing>
        </child>
      </object>
    </child>
  </object>
</interface>
